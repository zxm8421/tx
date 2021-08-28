#define TLOG_LOCAL_FILTER TLOG_D
#include "tlog.inc.h"

#if defined(__MINGW64__) || defined(__MINGW32__)
#define __USE_MINGW_ANSI_STDIO 1
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdarg.h>
#if defined(__MINGW64__) || defined(__MINGW32__)
#define _POSIX_THREAD_SAFE_FUNCTIONS
#endif
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <windows.h>
#endif

static ti tlog_filter = TLOG_ALL;
static volatile _Atomic ti64 tlog_seq = 0;
static ti tlog_fd = -1;
static ti tlog_test_fd = -1;
#if defined(__MINGW64__) || defined(__MINGW32__)
#else
#define __USE_GNU
#include <pthread.h>
static pthread_t tlog_tid = -1;
#endif

ti tlog_debug(const tc *format, ...)
{
	tc buf[TLOG_BUF_SIZE] = {0};
	ti len = 0;
	int fd = open("tlog_debug.log", O_CREAT | O_WRONLY | O_APPEND, 0644);

	if (fd < 0)
	{
		return -1;
	}
	else
	{
		va_list ap;
		va_start(ap, format);
		len += vsnprintf(buf, sizeof(buf), format, ap);
		va_end(ap);
		buf[len++] = '\n';

		len = write(fd, buf, len);
		close(fd);

		return len;
	};
}

tf tlog_watch(ti64 *watch)
{
	if (watch == tnull)
	{
		return -1;
	}

	struct timespec tp;
	clock_gettime(CLOCK_MONOTONIC, &tp);

	ti64 start = *watch;
	ti64 end = (ti64)tp.tv_sec * 1000 * 1000 * 1000 + (ti64)tp.tv_nsec;
	tf cost = (end - start) / 1e9;

	*watch = end;

	return cost;
}

ti64 tlog_getTimeNs()
{
	ti64 ns = 0;

	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);

	ns = (ti64)tp.tv_sec * 1000 * 1000 * 1000 + (ti64)tp.tv_nsec;

	return ns;
}

ti64 tlog_getTimeUs()
{
	ti64 us = tlog_getTimeNs() / 1000;

	return us;
}

ti64 tlog_getTimeMs()
{
	ti64 ms = tlog_getTimeNs() / 1000 / 1000;

	return ms;
}

ti64 tlog_getTime()
{
	ti64 s = tlog_getTimeNs() / 1000 / 1000 / 1000;

	return s;
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

	ti64 seq_now = atomic_fetch_add_explicit(&tlog_seq, 1, memory_order_relaxed);

	ti len = 0;
	tc buf[TLOG_BUF_SIZE] = {0};

	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);

	// "[20000101.000000.000 0]"
	{
		struct tm now;
		gmtime_r(&tp.tv_sec, &now);

		len += snprintf(buf + len, 64,
						"[%04d%02d%02d.%02d%02d%02d.%03ld %lld]",
						now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
						now.tm_hour, now.tm_min, now.tm_sec,
						tp.tv_nsec / 1000 / 1000,
						seq_now);
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

	if (TLOG_CONSOLE_ENABLE)
	{
#ifndef NTEST
		if (level >= TLOG_T)
		{
			ti ret __attribute__((unused)) = write(STDOUT_FILENO, buf, len);
		}
#else
		ti ret __attribute__((unused)) = write(STDOUT_FILENO, buf, len);
#endif
	}

	if (TLOG_FILE_ENABLE)
	{
		ti ret __attribute__((unused)) = write(tlog_fd, buf, len);

#ifndef NTEST
		if (level >= TLOG_T)
		{
			ti ret __attribute__((unused)) = write(tlog_test_fd, buf, len);
		}
#endif
	}

	return len;
}

ti tlog_rawhexdump(const tc *file, const ti line, const tc *func, const ti filter, const ti level, const tc *info, const void *ptr, const ti len)
{
	const tc hexTable[256 * 2 + 1] = "000102030405060708090A0B0C0D0E0F"
									 "101112131415161718191A1B1C1D1E1F"
									 "202122232425262728292A2B2C2D2E2F"
									 "303132333435363738393A3B3C3D3E3F"
									 "404142434445464748494A4B4C4D4E4F"
									 "505152535455565758595A5B5C5D5E5F"
									 "606162636465666768696A6B6C6D6E6F"
									 "707172737475767778797A7B7C7D7E7F"
									 "808182838485868788898A8B8C8D8E8F"
									 "909192939495969798999A9B9C9D9E9F"
									 "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
									 "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
									 "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
									 "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
									 "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
									 "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	ti le = len;
	tb *p = (tb *)ptr;

	tc buf[TLOG_BUF_SIZE] = {0};

	ti cur = 0;
	cur += snprintf(buf + cur, 256,
					"le = %dB, %s",
					le, info);

	for (ti i = 0; i < le; i++)
	{
		if ((i % 256) == 0)
		{
			buf[cur++] = '\n';
		}

		buf[cur++] = hexTable[p[i] * 2];
		buf[cur++] = hexTable[p[i] * 2 + 1];

		if (i != (le - 1))
		{
			if ((i % 16) == 15)
			{
				buf[cur++] = '\n';
			}
			else
			{
				buf[cur++] = ' ';
			}
		}
	}

	buf[cur++] = '\n';

	return tlog_rawprint(file, line, func, filter, level, "%s", buf);
}

void *tlog_thread_run(void *arg __attribute__((unused)))
{

	while (true)
	{
		usleep(1000 * 200);

		struct stat statbuf;
		stat(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", &statbuf);
		tisize size = statbuf.st_size;

		if (size >= TLOG_FILE_SIZE)
		{
			for (ti i = (TLOG_FILE_NUM - 2); i >= 0; i--)
			{
				tc filename_old[FILENAME_MAX] = {0};
				tc filename_new[FILENAME_MAX] = {0};
				snprintf(filename_old, sizeof(filename_old),
						 "%s.%d.log",
						 TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i);
				snprintf(filename_new, sizeof(filename_new),
						 "%s.%d.log",
						 TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i + 1);
				rename(filename_old, filename_new);
			}
			int fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(fd, tlog_fd);
			close(fd);
		}
	}

	return NULL;
}

ti tlog_init()
{
	static bool init = false;
	if (init)
	{
		return 0;
	}
	init = true;

#if defined(__MINGW64__) || defined(__MINGW32__)
	SetConsoleOutputCP(CP_UTF8);
	// FreeConsole();
#endif

	if (TLOG_FILE_ENABLE == 0)
	{
		return 0;
	}

#if defined(__MINGW64__) || defined(__MINGW32__)
	struct stat statbuf;
	stat(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", &statbuf);
	tisize size = statbuf.st_size;

	if (size >= TLOG_FILE_SIZE)
	{
		for (ti i = (TLOG_FILE_NUM - 2); i >= 0; i--)
		{
			tc filename_old[FILENAME_MAX] = {0};
			tc filename_new[FILENAME_MAX] = {0};
			snprintf(filename_old, sizeof(filename_old),
					 "%s.%d.log",
					 TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i);
			snprintf(filename_new, sizeof(filename_new),
					 "%s.%d.log",
					 TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i + 1);
			rename(filename_old, filename_new);
		}
	}

	ti ret __attribute__((unused)) = tlog_system("mkdir " TLOG_FILE_DIR);
	tlog_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
#ifndef NTEST
	tlog_test_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".test.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
#endif
#else
	ti ret __attribute__((unused)) = system("mkdir -m 755 -p " TLOG_FILE_DIR);
	tlog_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
#ifndef NTEST
	tlog_test_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".test.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
#endif

	pthread_create(&tlog_tid, NULL, tlog_thread_run, tnull);
	pthread_setname_np(tlog_tid, "tlog");

	tlog(TLOG_D, "当前 pid = %d, tid = %p", getpid(), pthread_self());
	tlog(TLOG_D, "创建tlog线程 tid = %p", tlog_tid);
#endif

	return 0;
}
