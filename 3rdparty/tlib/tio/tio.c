/**
 * @file tio.c
 * 
 * @brief 
 * 
 */
#include "tio.h"

ti tformat(tc *buf, ti n, const tc *format, ...)
{
	ti len = -1;

	va_list ap;
	va_start(ap, format);
	len = tvformat(buf, n, format, ap);
	va_end(ap);

	return len;
}

ti tvformat(tc *buf, ti n, const tc *format, va_list ap)
{
	ti len = -1;

	return len;
}
