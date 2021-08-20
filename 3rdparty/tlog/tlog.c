#define TLOG_LOCAL_FILTER TLOG_D
#include "tlog.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdarg.h>
#define _POSIX_THREAD_SAFE_FUNCTIONS
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#define __USE_GNU
#include <pthread.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <windows.h>
#endif

#define TLOG_THREAD_IDLE_MS 1
#define TLOG_BURST_SIZE 1024
#define TLOG_BUF_SIZE 4096

static ti tlog_filter = TLOG_GLOBAL_FILTER;
static ti tlog_fd = -1;

static struct
{
	volatile _Atomic ti64 seq_r;
	volatile _Atomic ti64 seq_w;

	struct
	{
		ti64 seq;
		ti32 len;
		tc s[TLOG_BUF_SIZE];
	} buf[TLOG_BURST_SIZE];
} tlog;

ti64 tlog_getTimeMs()
{
	ti64 ms = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);

	ms = (ti64)tv.tv_sec * 1000 + (ti64)tv.tv_usec / 1000;

	return ms;
}

ti64 tlog_getTimeUs()
{
	ti64 us = 0;
	struct timeval tv;
	gettimeofday(&tv, NULL);

	us = (ti64)tv.tv_sec * 1000 * 1000 + (ti64)tv.tv_usec;

	return us;
}

ti tlog_set_global_filter(ti filter)
{
	tlog_filter = filter;
	return tlog_filter;
}

tc *tlog_basename(const tc *path)
{
	if (path == tnull)
	{
		return tnull;
	}

	tc *p1 = strrchr(path, '/');
	tc *p2 = strrchr(path, '\\');

	tc *p = (p1 > p2) ? (p1) : (p2);
	p = (p == tnull) ? ((tc *)path) : (p + 1);

	return p;
}

ti tlog_system(const tc *cmd)
{
	ti ret = 0;
#if defined(__MINGW64__) || defined(__MINGW32__)
	wchar_t wcmd[2048] = L"";
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, cmd, -1, wcmd, tx_array_size(wcmd));
	ret = _wsystem(wcmd);
#else
	ret = system(cmd);
#endif

	return ret;
}

ti tlog_rawprint(const tc *file, const ti line, const tc *func, const ti filter, const ti level, const tc *format, ...)
{
	if ((TLOG_CONSOLE_ENABLE == 0) && (TLOG_FILE_ENABLE == 0))
	{
		return 0;
	}

	if ((level < filter) || (level < tlog_filter))
	{
		return 0;
	}

	if ((tlog.seq_w - tlog.seq_r) > TLOG_BURST_SIZE)
	{
		return -1;
	}

	ti64 seq_now = atomic_fetch_add_explicit(&tlog.seq_w, 1, memory_order_relaxed);

	ti len = 0;
	tc *buf = tlog.buf[seq_now % TLOG_BURST_SIZE].s;

	ti64 us = tlog_getTimeUs();

	// "[20000101.000000.000 0]"
	{
		time_t rawtime = us / (1000 * 1000);
		struct tm now;
		if (TLOG_USE_UTC)
		{
			gmtime_r(&rawtime, &now);

#if defined(__MINGW64__) || defined(__MINGW32__)
			len += snprintf(buf + len, 64,
							"[%04d%02d%02d.%02d%02d%02d.%06I64d %I64d]",
							now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
							now.tm_hour, now.tm_min, now.tm_sec,
							us % (1000 * 1000),
							seq_now);
#else
			len += snprintf(buf + len, 64,
							"[%04d%02d%02d.%02d%02d%02d.%06lld %lld]",
							now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
							now.tm_hour, now.tm_min, now.tm_sec,
							us % (1000 * 1000),
							seq_now);
#endif
		}
		else
		{
			localtime_r(&rawtime, &now);

#if defined(__MINGW64__) || defined(__MINGW32__)
			len += snprintf(buf + len, 64,
							"[%04d%02d%02d.%02d%02d%02d.%06I64d%+03ld%02ld %I64d]",
							now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
							now.tm_hour, now.tm_min, now.tm_sec,
							us % (1000 * 1000),
							(-timezone) / 3600, (-timezone) % 3600,
							seq_now);
#else
			len += snprintf(buf + len, 64,
							"[%04d%02d%02d.%02d%02d%02d.%06lld%+03ld%02ld %lld]",
							now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
							now.tm_hour, now.tm_min, now.tm_sec,
							us % (1000 * 1000),
							(-timezone) / 3600, (-timezone) % 3600,
							seq_now);
#endif
		}
	}

	// "[D/main.cpp:32 main]"
	{
		const tc level_table[] = {'D', 'I', 'W', 'E', 'T'};
		len += snprintf(buf + len, 256,
						"[%c/%s:%d %s]",
						level_table[level],
						tlog_basename(file), line,
						func);
	}

	// 用户输入
	{
		va_list ap;
		va_start(ap, format);
		len += vsnprintf(buf + len, sizeof(buf) - len, format, ap);
		va_end(ap);

		len += snprintf(buf + len, 2, "\n");
	}

	tlog.buf[seq_now % TLOG_BURST_SIZE].len = len;
	tlog.buf[seq_now % TLOG_BURST_SIZE].seq = seq_now;

	return len;
}

ti tlog_rawhexdump(const tc *file, const ti line, const tc *func, const ti filter, const ti level, const tc *info, const void *ptr, const ti len)
{
	ti le = len;
	tb *p = (tb *)ptr;

	tc buf[TLOG_BUF_SIZE] = {0};

	ti cur = 0;
	cur += snprintf(buf + cur, 256,
					"\n"
					"le = %dB, %s\n"
					"     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n",
					le, info);

	for (ti i = 0; i < le; i++)
	{
		if ((i % 16) == 0)
		{
			cur += snprintf(buf + cur, 6,
							"%04X ",
							i);
		}

		if (i == (le - 1))
		{
			cur += snprintf(buf + cur, 3,
							"%02X",
							p[i]);
		}
		else
		{
			if ((i % 16) == 15)
			{
				cur += snprintf(buf + cur, 4,
								"%02X\n",
								p[i]);
			}
			else
			{
				cur += snprintf(buf + cur, 4,
								"%02X ",
								p[i]);
			}
		}
	}

	cur += snprintf(buf + cur, 2, "\n");

	return tlog_rawprint(file, line, func, filter, level, "%s", buf);
}

void *tlog_thread(void *arg __attribute__((unused)))
{
#if defined(__MINGW64__) || defined(__MINGW32__)
	ti ret __attribute__((unused)) = tlog_system("mkdir " TLOG_FILE_DIR);
#else
	ti ret __attribute__((unused)) = system("mkdir -m 755 -p " TLOG_FILE_DIR);
#endif
	tlog_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);

	while (true)
	{
		ti64 seq_size = tlog.seq_w - tlog.seq_r;

		if (seq_size <= 0)
		{
			usleep(TLOG_THREAD_IDLE_MS * 1000);
		}
		else if (seq_size < TLOG_BURST_SIZE)
		{
			ti64 seq_now = atomic_load_explicit(&tlog.seq_r, memory_order_relaxed);

			if (seq_now == tlog.buf[seq_now % TLOG_BURST_SIZE].seq)
			{
				atomic_fetch_add_explicit(&tlog.seq_r, 1, memory_order_relaxed);
				tc *buf = tlog.buf[seq_now % TLOG_BURST_SIZE].s;
				ti len = tlog.buf[seq_now % TLOG_BURST_SIZE].len;

				if (TLOG_CONSOLE_ENABLE)
				{
					ti ret __attribute__((unused)) = write(STDOUT_FILENO, buf, len);
				}

				if (TLOG_FILE_ENABLE)
				{
					ti64 ret __attribute__((unused)) = write(tlog_fd, buf, len);
					if (lseek(tlog_fd, 0, SEEK_CUR) >= TLOG_FILE_SIZE)
					{
						close(tlog_fd);
						for (ti i = (TLOG_FILE_NUM - 2); i >= 0; i--)
						{
							tc filename_old[256] = {0};
							tc filename_new[256] = {0};
							snprintf(filename_old, sizeof(filename_old),
									 "%s.%d.log",
									 TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i);
							snprintf(filename_new, sizeof(filename_new),
									 "%s.%d.log",
									 TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i + 1);
							rename(filename_old, filename_new);
						}
						tlog_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
					}
				}

				if (TLOG_ECHO_QPS)
				{
					static ti64 cnt = 0;
					static ti64 us_last = 0;
					static ti64 lost_last = 0;
					cnt++;
					if ((cnt % 20000) == 0)
					{
						ti64 us = tlog_getTimeUs();
						ti64 us_delta = us - us_last;
						us_last = us;

						ti qps = 20000 * 1e6 / us_delta;

						ti64 lost = seq_now - cnt + 1;
						ti lost_rate = (lost - lost_last) * 1e6 / us_delta;
						lost_last = lost;

						tc cmd[256] = {0};
						snprintf(cmd, sizeof(cmd), "echo lost = %d, qps = %d >> qps.txt", lost_rate, qps);
						tlog_system(cmd);
					}
				}
			}
		}
		else if (seq_size < 2 * TLOG_BURST_SIZE)
		{
			atomic_fetch_add_explicit(&tlog.seq_r, 16, memory_order_relaxed);
		}
		else
		{
			atomic_fetch_add_explicit(&tlog.seq_r, TLOG_BURST_SIZE, memory_order_relaxed);
		}
	}

	return NULL;
}

ti tlog_init()
{
	pthread_t tid = -1;
	pthread_create(&tid, NULL, tlog_thread, tnull);
	pthread_setname_np(tid, "tlog");

	tlog(TLOG_D, "当前 pid = %d, tid = %lu", getpid(), pthread_self());
	tlog(TLOG_D, "创建tlog线程 tid = %lu", tid);

	return 0;
}