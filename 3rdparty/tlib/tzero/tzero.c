/**
 * @brief 
 * 
 * @file tzero.c
 */
#include "tzero.h"

#if defined(__MINGW64__) || defined(__MINGW32__)
#define __USE_MINGW_ANSI_STDIO 1
#endif
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#if defined(__MINGW64__) || defined(__MINGW32__)
#define _POSIX_
#endif
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <windows.h>
#endif

tf tlib_watch(ti64 *watch)
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

ti64 tlib_getTime_ns()
{
	ti64 ns = 0;

	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);

	ns = (ti64)tp.tv_sec * 1000 * 1000 * 1000 + (ti64)tp.tv_nsec;

	return ns;
}

ti64 tlib_getTime_us()
{
	ti64 us = tlib_getTime_ns() / 1000;

	return us;
}

ti64 tlib_getTime_ms()
{
	ti64 ms = tlib_getTime_ns() / 1000 / 1000;

	return ms;
}

tf tlib_getTime()
{
	tf ts = tlib_getTime_ns() / 1e9;

	return ts;
}

tc *tlib_basename(const tc path[])
{
	if (path == tnull)
	{
		return tnull;
	}

	tc *token1 = strrchr(path, '/');
	tc *token2 = strrchr(path, '\\');

	tc *token = (token1 > token2) ? (token1) : (token2);

	token = (token == tnull) ? ((tc *)path) : (token + 1);

	return token;
}

ti tlib_system(const tc *format, ...)
{
	tc cmd[_POSIX_ARG_MAX] = "";

	ti ret = 0;

	va_list ap;
	va_start(ap, format);
	vsnprintf(cmd, sizeof(cmd), format, ap);
	va_end(ap);

#if defined(__MINGW64__) || defined(__MINGW32__)
	wchar_t wcmd[_POSIX_ARG_MAX] = L"";
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, cmd, -1, wcmd, tx_array_size(wcmd));
	ret = _wsystem(wcmd);
#else
	ret = system(cmd);
#endif

	return ret;
}

tu32 tlib_hash_str(const tc str[])
{
	// FNV hash, http://www.isthe.com/chongo/tech/comp/fnv/index.html
	const tu32 basis = 2166136261;
	const tu32 prime = 16777619;

	tu32 hash = basis;
	tc *cur = (tc *)str;

	if (cur == tnull)
	{
		return hash;
	}

	while (*cur)
	{
		hash ^= *cur;
		hash *= prime;
		cur++;
	}

	return hash;
}

ti tlib_atox(tc c)
{
	ti x = 0;

	switch (c)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		x = c - '0';
		break;

	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		x = c - 'A' + 0xA;
		break;

	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		x = c - 'a' + 0xA;
		break;

	default:
		x = -1;
		break;
	}

	return x;
}

tc tlib_xtoa(ti x)
{
	tc c = '\0';

	switch (x)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		c = x + '0';
		break;

	case 0xA:
	case 0xB:
	case 0xC:
	case 0xD:
	case 0xE:
	case 0xF:
		c = x - 0xA + 'A';
		break;

	default:
		c = '\0';
		break;
	}

	return c;
}

ti tlib_byteToHex(const tb byte[], const ti len, tc hex[])
{
	if (byte == tnull)
	{
		return -1;
	}

	if (len < 0)
	{
		return -1;
	}

	ti ret = 2 * len;

	if (hex != tnull)
	{
		for (ti i = 0; i < len; i++)
		{
			hex[2 * i] = tlib_xtoa((byte[i] >> 4) & 0x0F);
			hex[2 * i + 1] = tlib_xtoa(byte[i] & 0x0F);
		}
		hex[2 * len] = '\0';
	}

	return ret;
}

ti tlib_hexToByte(const tc hex[], const ti len, tb byte[])
{
	if (hex == tnull)
	{
		return -1;
	}

	if (len < 0)
	{
		return -1;
	}

	if (len % 2 != 0)
	{
		return -1;
	}

	ti ret = len / 2;
	if (byte != tnull)
	{
		for (ti i = 0; i < len; i = i + 2)
		{
			ti byte_H = tlib_atox(hex[i]);
			ti byte_L = tlib_atox(hex[i + 1]);
			if ((byte_H < 0) || (byte_L < 0))
			{
				return -1;
			}

			byte[i / 2] = (byte_H << 4) + byte_L;
		}
	}

	return ret;
}

// struct tlib_format_Info
// {
// 	tc spec[64];
// 	ti size; // 注意类型提升, 0 表示长度可变
// 	tlib_format_Handle handle;
// };

// struct tlib_format_Info tlib_format_info[] = {
// 	{"u8", 4, &tlib_vformat_u8},
// 	{"i8", 4, &tlib_vformat_i8},
// 	tnull,
// };

// struct tlib_format_Arg
// {
// 	void *p;
// 	ti size;
// };

// struct tlib_format_Arg tlib_format_arg[10] = {
// 	{},
// 	{},
// 	tnull,
// };

// ti tlib_format(tc *buf, const tc *format, ...)
// {
// 	ti len = 0;

// 	va_list ap;
// 	va_start(ap, format);
// 	len = tlib_vformat(buf, format, ap);
// 	va_end(ap);

// 	return len;
// }

// ti tlib_vformat(tc *buf, const tc *format, va_list ap)
// {
// 	ti len = 0;

// 	void *pv = va_arg(ap, int);
// 	len += tlib_format_u8(buf, NULL, ap);

// 	return len;
// }

// ti tlib_vformat_u8(tc *buf, const struct tlib_format_info *info, const void *const pv)
// {
// 	tlog_hexdump(TLOG_D, "", pv, 32);

// 	ti len = 0;
// 	tc *cur = buf;

// 	tu8 *v = (tu8 *)pv;

// 	tc n[4] = "000";

// 	n[0] = tlib_xtoa((*v) % 10);
// 	n[1] = tlib_xtoa((*v) / 10 % 10);
// 	n[2] = tlib_xtoa((*v) / 100 % 10);

// 	buf[len++] = n[2];
// 	buf[len++] = n[1];
// 	buf[len++] = n[0];

// 	return len;
// }
