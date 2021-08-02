/**
 * @brief 数据类型定义
 * 
 * @file ttype.h
 */
#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#if __SIZEOF_POINTER__ == 8
#define tp_size 8
#elif __SIZEOF_POINTER__ == 4
#define tp_size 4
#else
#define tp_size 4
#endif

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

#if __SIZEOF_POINTER__ == 8
	typedef tu64 tsize;
#elif __SIZEOF_POINTER__ == 4
typedef tu32 tsize;
#else
typedef tu32 tsize;
#endif

	typedef ti32 terr;

// 布尔
#define ttrue (1)
#define tfalse (0)

#define tnull ((void *)0)

#ifdef __cplusplus
}
#endif