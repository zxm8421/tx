/**
 * @file tstring.c
 * 
 * @brief 
 * 
 */
#include "tstring.h"

#include <stdlib.h>
#include <string.h>

#include <tlog/tlog.h>

tString *tString_new(const tc *s)
{
	tString *self = (tString *)calloc(1, sizeof(tString));

	self->len = strlen(s);
	if (self->len > 0)
	{
		self->size = self->len + 1;
		self->data = (tc *)calloc(self->size, sizeof(tc));
		strcpy(self->data, s);
	}

	return self;
}

tString *tString_del(tString *self)
{
	tString_clear(self);

	free(self);
	self = tnull;

	return self;
}

tString *tString_clear(tString *self)
{
	if (self->data != tnull)
	{
		free(self->data);
		self->data = tnull;
	}

	return self;
}

ti tString_reserve(tString *self, ti size)
{
	if (size < 64)
	{
		size = 64;
	}
	else if (size < 128)
	{
		size = 128;
	}
	else if (size < 256)
	{
		size = 256;
	}
	else if (size < 512)
	{
		size = 512;
	}
	else if (size < 1024)
	{
		size = 1024;
	}
	else if (size < 2048)
	{
		size = 2048;
	}
	else if (size < 4096)
	{
		size = 4096;
	}
	else
	{
		size = (size + (4096 - 1)) / 4096;
	}

	if (size > self->size)
	{
		tc *data = (tc *)realloc(self->data, size);
		if (data != tnull)
		{
			self->data = data;
			if (self->size == 0)
			{
				memset(self->data, 0, size);
			}
			else
			{
				memset(self->data + self->size, 0, size - self->size);
			}
			self->size = size;
		}
	}

	return self->size;
}

ti tString_len(tString *self)
{
	return self->len;
}

ti tString_size(tString *self)
{
	return self->size;
}

tString *tString_dup(tString *src)
{
	tString *self = tString_new("");

	tString_cpy(self, src);

	return self;
}

tString *tString_cpy(tString *dst, tString *src)
{
	if (src->len == 0)
	{
		return dst;
	}

	tString_reserve(dst, src->len + 1);
	strcpy(dst->data, src->data);
	dst->len = src->len;

	return dst;
}

tString *tString_cat(tString *dst, tString *src)
{
	if (src->len == 0)
	{
		return dst;
	}

	tString_reserve(dst, dst->len + src->len + 1);
	strcpy(dst->data, src->data);
	dst->len += src->len;

	return dst;
}

ti tString_cmp(tString *s1, tString *s2)
{
	return strcmp(s1->data, s2->data);
}
