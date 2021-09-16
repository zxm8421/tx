/**
 * @file tio.h
 * 
 * @brief 
 * 
 */
#pragma once

#if defined(__MINGW64__) || defined(__MINGW32__)
#define __USE_MINGW_ANSI_STDIO 1
#endif
#include <stdarg.h>
#include <stdio.h>

#include <ttype/ttype.h>

/*******************************************************************************
 * @par export
 */
#ifdef __cplusplus
extern "C"
{
#endif
	ti tprint(ti n, const tc *format, ...);
	ti tformat(tc *buf, ti n, const tc *format, ...);
	ti tvformat(tc *buf, ti n, const tc *format, va_list ap);

	ti tscan(const tc *format, ...);
	ti tfscan();
	ti tparse(const tc *buf, const tc *format, ...);
	ti tvparse(const tc *buf, const tc *format, va_list ap);

	// less is more
	// "{:u8}"
	// "{:u8*:}"
	// "{:u8[*]:}"
	// "{:u8[12]:}"
	// "{:u8:^ }"
	// "{:u8[*]:^+03.4}"
	// "{:u8[10_]:^+03.4}"
	// "{:u8[*,]e:^+03.4}"
	// "{:u8[*,]#x:^+03.4}"
	// "{:time::YYYYMMDD.HHMMSS}"
	// 转义 // /{ /} /:	/[ /]
	typedef struct tformat_Info
	{
		ti pos;	 // 参数位置
		ti size; // 参数类型注册的大小

		tc *spec;	// 类型说明符
		tc hashtag; // #扩展, [\0#]
		tc alt;		// 类型辅助说明符 [\0eEbBoOdDxX]

		tc array;	 // 是否是数组 [\0*]
		ti arrayLen; // 数组元素个数
		tc gap;		 // 数组各元素之间的间隔字符, 最后一个元素后无此字符, 默认[ ],可以为 [ ,_]

		tc align; // 对齐方式, [\0<^>]
		tc sign;  // 符号, [\0+-]
		tc pad;	  // 填充字符 [\0 0_]
		ti width; // 宽度
		ti prec;  // 精度

		tc *extra; // 自定义选项
	} tformat_Info;

#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * @par impl
 */
#ifdef __cplusplus
extern "C"
{
#endif

	ti tformat_none(const tformat_Info *info, va_list ap);
	ti tformat_e(tc *buf, const tc *format, va_list ap);
	ti tformat_p(tc *buf, const tc *format, va_list ap);

	ti tformat_u8(tc *buf, const tc *format, va_list ap);
	ti tformat_i8(tc *buf, const tc *format, va_list ap);
	ti tformat_u16(tc *buf, const tc *format, va_list ap);
	ti tformat_i16(tc *buf, const tc *format, va_list ap);
	ti tformat_u32(tc *buf, const tc *format, va_list ap);
	ti tformat_i32(tc *buf, const tc *format, va_list ap);
	ti tformat_u64(tc *buf, const tc *format, va_list ap);
	ti tformat_i64(tc *buf, const tc *format, va_list ap);

	ti tformat_usize(tc *buf, const tc *format, va_list ap);
	ti tformat_isize(tc *buf, const tc *format, va_list ap);

	ti tformat_i(tc *buf, const tc *format, va_list ap);

	ti tformat_f32(tc *buf, const tc *format, va_list ap);
	ti tformat_f64(tc *buf, const tc *format, va_list ap);
	ti tformat_f(tc *buf, const tc *format, va_list ap);

	ti tformat_b(tc *buf, const tc *format, va_list ap);
	ti tformat_b8(tc *buf, const tc *format, va_list ap);
	ti tformat_b16(tc *buf, const tc *format, va_list ap);
	ti tformat_b32(tc *buf, const tc *format, va_list ap);
	ti tformat_b64(tc *buf, const tc *format, va_list ap);

	ti tformat_c(tc *buf, const tc *format, va_list ap);
	ti tformat_c8(tc *buf, const tc *format, va_list ap);
	ti tformat_c16(tc *buf, const tc *format, va_list ap);
	ti tformat_c32(tc *buf, const tc *format, va_list ap);

	ti tformat_wc(tc *buf, const tc *format, va_list ap);

	ti tformat_time(tc *buf, const tc *format, va_list ap);

	ti tformat_watch(tc *buf, const tc *format, va_list ap);

	ti tvparse_u8(const tc *buf, const tc *format, va_list ap);

#ifdef __cplusplus
}
#endif
