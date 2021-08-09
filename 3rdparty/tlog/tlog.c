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
#include <pthread.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <windows.h>
#endif

static ti tlog_console_level = TLOG_CONSOLE_LEVEL;
static ti tlog_file_level = TLOG_FILE_LEVEL;
static int tlog_fd = -1;

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

ti tlog_getTimeStr(const ti64 us, ti use_utc, tc *timeStr)
{
	ti len = 0;
	time_t rawtime = us / (1000 * 1000);
	struct tm now;

	if (use_utc != 0)
	{
		gmtime_r(&rawtime, &now);
#if defined(__MINGW64__) || defined(__MINGW32__)

		len = snprintf((char *)timeStr, 64,
					   "%04d%02d%02d.%02d%02d%02d.%06I64d",
					   now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
					   now.tm_hour, now.tm_min, now.tm_sec,
					   us % (1000 * 1000));
#else
		len = snprintf((char *)timeStr, 64,
					   "%04d%02d%02d.%02d%02d%02d.%06lld",
					   now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
					   now.tm_hour, now.tm_min, now.tm_sec,
					   us % (1000 * 1000));
#endif
	}
	else
	{
		localtime_r(&rawtime, &now);
#if defined(__MINGW64__) || defined(__MINGW32__)
		len = snprintf((char *)timeStr, 64,
					   "%04d%02d%02d.%02d%02d%02d.%06I64d %+03ld%02ld",
					   now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
					   now.tm_hour, now.tm_min, now.tm_sec,
					   us % (1000 * 1000),
					   (-timezone) / 3600, (-timezone) % 3600);
#else
		len = snprintf((char *)timeStr, 64,
					   "%04d%02d%02d.%02d%02d%02d.%06lld %+03ld%02ld",
					   now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
					   now.tm_hour, now.tm_min, now.tm_sec,
					   us % (1000 * 1000),
					   (-timezone) / 3600, (-timezone) % 3600);
#endif
	}

	return len;
}

ti tlog_try_rotate(ti64 limit)
{
	static pthread_mutex_t tlog_rotate_mutex = PTHREAD_MUTEX_INITIALIZER;

	if (limit < 0)
	{
		return 0;
	}

	if (pthread_mutex_trylock(&tlog_rotate_mutex) == 0)
	{
		struct stat statbuf;
		stat(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", &statbuf);
		ti64 size = statbuf.st_size;

		if ((size > TLOG_FILE_SIZE) || (size > limit))
		{
			for (ti i = (TLOG_FILE_NUM - 2); i >= 0; i--)
			{
				tc filename_old[256] = {0};
				tc filename_new[256] = {0};
				sprintf((char *)filename_old,
						"%s.%d.log",
						TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i);
				sprintf((char *)filename_new,
						"%s.%d.log",
						TLOG_FILE_DIR "/" TLOG_FILE_PREFIX, i + 1);
				rename((char *)filename_old, (char *)filename_new);
			}
			int fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
			dup2(fd, tlog_fd);
			close(fd);
		}
		pthread_mutex_unlock(&tlog_rotate_mutex);
	}

	return 0;
}

ti tlog_set_console_level(ti level)
{
	tlog_console_level = level;
	return tlog_console_level;
}

ti tlog_set_file_level(ti level)
{
	tlog_file_level = level;
	return tlog_file_level;
}

ti tlog_system(const tc *cmd)
{
	ti ret = 0;
#if defined(__MINGW64__) || defined(__MINGW32__)
	wchar_t wcmd[2048] = L"";
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (char *)cmd, -1, wcmd, tx_array_size(wcmd));
	ret = _wsystem(wcmd);
#else
	ret = system((char *)cmd);
#endif

	return ret;
}

static const tc tlog_level_str[][2] = {"D", "I", "W", "E"};
ti tlog_print(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, const tc *format, ...)
{
	if (level < tag_level)
	{
		return 0;
	}

	if ((level < tlog_console_level) && (level < tlog_file_level))
	{
		return 0;
	}

	tc tlog_buf[TLOG_BUF_SIZE] = {0};

	static volatile _Atomic ti64 seq = 0;
	ti64 seq_now = atomic_fetch_add_explicit(&seq, 1, memory_order_relaxed);
	ti len = 0;

	tc timeStr[64] = {0};
	ti64 us = tlog_getTimeUs();
	tlog_getTimeStr(us, TLOG_USE_UTC, timeStr);

#if 0
	{
		static ti64 seq_last = 0;
		static ti64 us_last = 0;
		if ((us - us_last) >= (1000 * 1000))
		{
			ti64 us_delta = us - us_last;
			us_last = us;

			ti64 seq_delta = seq_now - seq_last;
			seq_last = seq_now;

			ti64 qps = seq_delta * 1000 * 1000 / us_delta;

			tc cmd[256] = {0};
#if defined(__MINGW64__) || defined(__MINGW32__)
			sprintf((char *)cmd, "echo \"---------- tlog[%s %I64d] qps = %I64d ----------\" >> qps.txt", timeStr, seq_now, qps);
			ti ret __attribute__((unused)) = tlog_system(cmd);
#else
			sprintf((char *)cmd, "echo \"---------- tlog[%s %lld] qps = %lld ----------\" >> qps.txt", timeStr, seq_now, qps);
			ti ret __attribute__((unused)) = system((char *)cmd);
#endif
		}
	}
#endif

#if defined(__MINGW64__) || defined(__MINGW32__)
	tc *filename = (tc *)strrchr((char *)file, '\\');
#else
	tc *filename = (tc *)strrchr((char *)file, '/');
#endif

	if (filename != tnull)
	{
		filename = filename + 1;
	}
	else
	{
		filename = (tc *)file;
	}

#if defined(__MINGW64__) || defined(__MINGW32__)
	len = sprintf((char *)tlog_buf,
				  "[%s %I64d][%s/%s][%s:%d][%s]",
				  (char *)timeStr, seq_now,
				  (char *)tlog_level_str[level], (char *)tag,
				  (char *)filename, line, (char *)func);
#else
	len = sprintf((char *)tlog_buf,
				  "[%s %lld][%s/%s][%s:%d][%s]",
				  (char *)timeStr, seq_now,
				  (char *)tlog_level_str[level], (char *)tag,
				  (char *)filename, line, (char *)func);
#endif

	va_list args;
	va_start(args, format);
	len += vsprintf((char *)(tlog_buf + len), (char *)format, args);
	tlog_buf[len] = '\n';
	tlog_buf[len + 1] = '\0';
	len += 1;
	va_end(args);

	if ((level >= tlog_console_level) && (level >= tag_level))
	{
		ti ret __attribute__((unused)) = write(STDOUT_FILENO, tlog_buf, len);
	}

	if ((level >= tlog_file_level) && (level >= tag_level))
	{
		if (tlog_fd < 0)
		{
#if defined(__MINGW64__) || defined(__MINGW32__)
			ti ret __attribute__((unused)) = tlog_system((tc *)"mkdir " TLOG_FILE_DIR);
#else
			ti ret __attribute__((unused)) = system("mkdir -m 755 -p " TLOG_FILE_DIR);
#endif
			tlog_fd = open(TLOG_FILE_DIR "/" TLOG_FILE_PREFIX ".0.log", O_CREAT | O_WRONLY | O_APPEND, 0644);
		}

		if (tlog_fd >= 0)
		{
			ti64 ret __attribute__((unused)) = write(tlog_fd, tlog_buf, len);
			ti64 size = lseek(tlog_fd, 0, SEEK_CUR);
			if (size >= TLOG_FILE_SIZE)
			{
				tlog_try_rotate(TLOG_FILE_SIZE);
			}
		}
	}

	return len;
}

static const tc tlog_hex2tc[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
ti tlog_print_hexdump(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, const tc *info, const void *ptr, ti len)
{
	if (len > 512)
	{
		len = 512;
	}

	tc tlog_buf[TLOG_BUF_SIZE] = {0};

	ti cur = sprintf((char *)tlog_buf,
					 "\n     00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F %dB %s\n",
					 len, info);
	ti row = (len + (16 - 1)) / 16;
	ti col = len % 16;
	if (col == 0)
	{
		col = 16;
	}

	for (ti i = 0; i < row; i++)
	{
		cur += sprintf((char *)tlog_buf + cur, "%04X ", i * 16);
		for (ti j = 0; j < 16; j++)
		{
			if ((i != (row - 1)) || (j < col))
			{
				tb hex = ((tb(*)[16])ptr)[i][j];
				tb hex_H = (hex & 0xF0) >> 4;
				tb hex_L = hex & 0x0F;
				tlog_buf[cur] = tlog_hex2tc[hex_H];
				tlog_buf[cur + 1] = tlog_hex2tc[hex_L];
				tlog_buf[cur + 2] = ' ';

				if ((hex >= 0x20) && (hex <= 0x7E))
				{
					tlog_buf[cur + 3 * (16 - j) + j] = hex;
				}
				else
				{
					tlog_buf[cur + 3 * (16 - j) + j] = ' ';
				}
			}
			else
			{
				tlog_buf[cur] = ' ';
				tlog_buf[cur + 1] = ' ';
				tlog_buf[cur + 2] = ' ';
				tlog_buf[cur + 3 * (16 - j) + j] = ' ';
			}
			cur += 3;
		}
		cur += 16;
		tlog_buf[cur] = '\n';
		cur += 1;
	}
	tlog_buf[cur] = '\0';

	ti ret = tlog_print(tag, tag_level, level, file, line, func, (const tc *)"%s", tlog_buf);
	return ret;
}