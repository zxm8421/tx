/**
 * @file tstring.h
 * 
 * @brief 
 * 
 */
#pragma once

#include <ttype/ttype.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct tString
	{
		ti len;
		tc *data;

		ti size;
	} tString;

	tString *tString_new(const tc *s);
	tString *tString_del(tString *self);

	tString *tString_clear(tString *self);

	ti tString_reserve(tString *self, ti size);

	ti tString_len(tString *self);
	ti tString_size(tString *self);

	tString *tString_dup(tString *src);
	tString *tString_cpy(tString *dst, tString *src);
	tString *tString_cat(tString *dst, tString *src);
	ti tString_cmp(tString *s1, tString *s2);

#ifdef __cplusplus
}
#endif
