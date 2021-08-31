/**
 * @brief 
 * 
 * @file tzero.c
 */
#include "tzero.in.h"

#include <string.h>
#if defined(__MINGW64__) || defined(__MINGW32__)
#define _POSIX_
#endif
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

ti64 tlob_getTime_ms()
{
	ti64 ms = tlib_getTime_us() / 1000 / 1000;

	return ms;
}

tf tlib_getTime()
{
	tf ts = tlib_getTime_ns() / 1e9;

	return ts;
}

tc *tlib_basename(const tc *path)
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

ti tlib_system(const tc *cmd)
{
	ti ret = 0;
#if defined(__MINGW64__) || defined(__MINGW32__)
	wchar_t wcmd[_POSIX_ARG_MAX] = L"";
	MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, cmd, -1, wcmd, tx_array_size(wcmd));
	ret = _wsystem(wcmd);
#else
	ret = system(cmd);
#endif

	return ret;
}

tu32 tlib_hash_str(const tc *str)
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
