/**
 * @file ttype.h
 * 
 * @brief 类型定义
 * 
 */
#pragma once

#include <wchar.h>

#define tx_size __SIZEOF_POINTER__

// 整数
typedef unsigned char tu8;
typedef signed char ti8;
typedef unsigned short tu16;
typedef signed short ti16;
typedef unsigned int tu32;
typedef signed int ti32;
typedef unsigned long long tu64;
typedef signed long long ti64;
typedef int ti;

// 浮点数
typedef float tf32;
typedef double tf64;
typedef double tf;

//字节
typedef tu8 tb8;
typedef tu16 tb16;
typedef tu32 tb32;
typedef tu64 tb64;
typedef tb8 tb;

//字符
typedef char tc;
typedef wchar_t twc;

typedef tb8 tc8;
typedef tb16 tc16;
typedef tb32 tc32;

//错误
typedef ti te;

// 布尔
#include <stdbool.h>
typedef bool tbo;
#define tbt true
#define tbf false

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

#define tx_unused(x) (void)x

#define tx_array_size(a) ((ti64)(sizeof(a) / sizeof(a[0])))

/**
 * @brief 类型枚举
 * 类型最大值应该<= 0b00111111, 即64
 * 
 */
enum tType
{
	tType_none = 0,

	tType_bo = 1,
	tType_e,
	tType_p,

	tType_u8 = 10,
	tType_i8,
	tType_u16,
	tType_i16,
	tType_u32,
	tType_i32,
	tType_i = tType_i32,
	tType_u64,
	tType_i64,

	tType_f32 = 20,
	tType_f64,
	tType_f = tType_f64,

	tType_b8 = 30,
	tType_b = tType_b8,
	tType_b16,
	tType_b32,
	tType_b64,

	tType_c,
	tType_wc,
	tType_c8,
	tType_c16,
	tType_c32,
};
