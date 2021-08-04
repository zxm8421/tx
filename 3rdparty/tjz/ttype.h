/**
 * @brief 数据类型定义
 * C17, C++17
 * @file ttype.h
 */
#pragma once

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
typedef ti32 ti;

// 浮点数
typedef float tf32;
typedef double tf64;
typedef tf64 tf;

//字节
typedef tu8 tb8;
typedef tu16 tb16;
typedef tu32 tb32;
typedef tu64 tb64;
typedef tb8 tb;

//字符
typedef tb8 tc8;
typedef tb16 tc16;
typedef tb32 tc32;
typedef tb64 tc64;
typedef tc8 tc;

//错误
typedef ti32 terr;

// 布尔
#include <stdbool.h>
typedef bool tbool;
#define ttrue true
#define tfalse false

// 指针
#if (tp_size == 8)
typedef tu64 tsize;
#else
typedef tu32 tsize;
#endif

#ifdef __cplusplus
#define tnull nullptr
#else
#define tnull ((void *)0)
#endif
