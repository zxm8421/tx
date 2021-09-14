/**
 * @file tio.h
 * 
 * @brief 
 * 
 */
#pragma once

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
	ti tprint(const tc *format, ...);
	ti tscan(const tc *format, ...);

	ti tfprint();
	ti tfscan();

	ti tformat(tc *buf, const tc *format, ...);
	ti tparse(const tc *buf, const tc *format, ...);

	ti tvformat(tc *buf, const tc *format, va_list ap);
	ti tvparse(const tc *buf, const tc *format, va_list ap);

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

	ti tvformat_none(tc *buf, const tc *format, va_list ap);
	ti tvformat_e(tc *buf, const tc *format, va_list ap);
	ti tvformat_p(tc *buf, const tc *format, va_list ap);

	ti tvformat_u8(tc *buf, const tc *format, va_list ap);
	ti tvformat_i8(tc *buf, const tc *format, va_list ap);
	ti tvformat_u16(tc *buf, const tc *format, va_list ap);
	ti tvformat_i16(tc *buf, const tc *format, va_list ap);
	ti tvformat_u32(tc *buf, const tc *format, va_list ap);
	ti tvformat_i32(tc *buf, const tc *format, va_list ap);
	ti tvformat_u64(tc *buf, const tc *format, va_list ap);
	ti tvformat_i64(tc *buf, const tc *format, va_list ap);

	ti tvformat_usize(tc *buf, const tc *format, va_list ap);
	ti tvformat_isize(tc *buf, const tc *format, va_list ap);

	ti tvformat_i(tc *buf, const tc *format, va_list ap);

	ti tvformat_f32(tc *buf, const tc *format, va_list ap);
	ti tvformat_f64(tc *buf, const tc *format, va_list ap);
	ti tvformat_f(tc *buf, const tc *format, va_list ap);

	ti tvformat_b(tc *buf, const tc *format, va_list ap);
	ti tvformat_b8(tc *buf, const tc *format, va_list ap);
	ti tvformat_b16(tc *buf, const tc *format, va_list ap);
	ti tvformat_b32(tc *buf, const tc *format, va_list ap);
	ti tvformat_b64(tc *buf, const tc *format, va_list ap);

	ti tvformat_c(tc *buf, const tc *format, va_list ap);
	ti tvformat_c8(tc *buf, const tc *format, va_list ap);
	ti tvformat_c16(tc *buf, const tc *format, va_list ap);
	ti tvformat_c32(tc *buf, const tc *format, va_list ap);

	ti tvformat_wc(tc *buf, const tc *format, va_list ap);

	ti tvformat_time(tc *buf, const tc *format, va_list ap);

	ti tvformat_watch(tc *buf, const tc *format, va_list ap);

	ti tvparse_u8(const tc *buf, const tc *format, va_list ap);

#ifdef __cplusplus
}
#endif
