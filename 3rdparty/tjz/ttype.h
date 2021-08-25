/**
 * @brief 数据类型定义
 * C17, C++17
 * @file ttype.h
 */
#pragma once

#include <wchar.h>

#define tp_size __SIZEOF_POINTER__

// 整数
typedef unsigned char tu8;
typedef signed char ti8;
typedef unsigned short tu16;
typedef signed short ti16;
typedef unsigned int tu32;
typedef signed int ti32;
typedef unsigned long long tu64;
typedef signed long long ti64;
#ifdef __SIZEOF_INT128__
typedef __uint128_t tu128;
typedef __int128_t ti128;
#endif
typedef signed long tisize;
typedef unsigned long tusize;
typedef int ti;

// 浮点数
typedef float tf32;
typedef double tf64;
#if __SIZEOF_LONG_DOUBLE__ == 16
typedef long double tf128;
#elif __SIZEOF_LONG_DOUBLE__ == 12
typedef long double tf96;
#else
#endif
typedef double tf;

//字节
typedef tu8 tb8;
typedef tu16 tb16;
typedef tu32 tb32;
typedef tu64 tb64;
typedef tusize tbsize;
typedef tb8 tb;

//字符
typedef tb8 tc8;
typedef tb16 tc16;
typedef tb32 tc32;
typedef tb64 tc64;
typedef tbsize tcsize;

typedef char tc;
typedef wchar_t twc;

//错误
typedef ti te;

// 布尔
#include <stdbool.h>

#ifdef __cplusplus
#define tnull nullptr
#else
#define tnull ((void *)0)
#endif

#define tstatic static
#ifdef __cplusplus
#define textern extern "C"
#else
#define textern extern
#endif

#define tx_array_size(a) ((tisize)(sizeof(a) / sizeof(a[0])))
