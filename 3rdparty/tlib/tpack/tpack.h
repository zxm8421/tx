/**
 * @file tpack.h
 * 
 * @brief 
 * 
 */
#pragma once

#include <tlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

	enum tpack_Type
	{
		tpack_Type_none = 0,
		tpack_Type_bo,	 // bool
		tpack_Type_i,	 // int
		tpack_Type_f,	 // float
		tpack_Type_s,	 // string
		tpack_Type_b,	 // bytes
		tpack_Type_pack, // tPack
	};

	typedef struct tPack
	{
		struct
		{
			ti len;
			tc *name;
		} key;

		struct
		{
			enum tpack_Type type;
			ti len;

			union
			{
				tbo bo;
				ti i;
				tf f;
				tc *s;
				tb *b;
				tPack *pack;
			};
		} value;

		tPack *prev;
		tPack *next;
	} tPack;

	tPack *tpack_loads(const tc *s, const ti len);
	ti tpack_dumps(tc *s);

	tPack *tpack_loadb(const tb *b, const ti len);
	ti tpack_dumpb(tb *b);

	tPack *tpack_new();
	tPack *tpack_new_tbo(tbo bo);
	tPack *tpack_new_ti(ti i);
	tPack *tpack_new_tf(tf f);
	tPack *tpack_new_tc(tc *s, ti len);
	tPack *tpack_new_tb(tb *b, ti len);
	tPack *tpack_del(tPack *self);

	tPack *tpack_clear(tPack *self);

	tPack *tpack_head(tPack *self);
	tPack *tpack_tail(tPack *self);

	tPack *tpack_at(tPack *self, ti pos);
	tPack *tpack_get(tPack *self, tc *key);
	tPack *tpack_find(tPack *self, tPack *pack);

	tPack *tpack_push(tPack *self, ti pos, tPack *pack);
	tPack *tpack_pop(tPack *self, ti pos);
	tPack *tpack_append(tPack *self, tPack *pack);

	ti tpack_to_ti(tPack *self);

#ifdef __cplusplus
}
#endif
