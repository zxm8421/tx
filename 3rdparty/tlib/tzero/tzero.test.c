/**
 * @brief 
 * 
 * @file tzero.test.c
 */
#include "tzero.h"

#include <ctype.h>
#include <limits.h>

#include <unistd.h>

#include <tlib.h>

ttest(test_tzero_tlib_getTime, 0)
{
	ttest_check_gt(tlib_getTime(), 0);
	ttest_check_gt(tlib_getTime_ms(), 0);
	ttest_check_gt(tlib_getTime_us(), 0);
	ttest_check_gt(tlib_getTime_ns(), 0);
}

ttest(test_tzero_tlib_watch, 0)
{
	ttest_check_ge(tlib_watch(tnull), -1);

	ti64 watch = 0;
	ttest_check_ge(tlib_watch(&watch), 0);
	ttest_check_gt(watch, 0);
	usleep(1000 * 20);
	tf cost = tlib_watch(&watch);
	ttest_check_gt(watch, 0);
	tlog(TLOG_T, "usleep(1000*2), cost = %.09f s", cost);
	ttest_check_in_range(cost, 0.01, 0.05);
}

ttest(test_tzero_tlib_basename, 0)
{
	ttest_check_null(tlib_basename(NULL));
	ttest_check_str_eq(tlib_basename("main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("/home/test/abc/main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("C:\\main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("C:\\桌面\\main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("C:\\桌面\\123/abc/main.c"), "main.c");
}

ttest(test_tzero_tlib_system, 0)
{
	ttest_check_eq(tlib_system("echo 0123456789"), 0);
}

ttest(test_tzero_tlib_hash_str, 0)
{
	ttest_check_ne(tlib_hash_str(tnull), 0);
	ttest_check_ne(tlib_hash_str(""), 0);
	ttest_check_ne(tlib_hash_str("0123456789ABCDEF"), 0);
}

ttest(test_tzero_tlib_atox, 0)
{
	ttest_check_eq(tlib_atox('0'), 0);
	ttest_check_eq(tlib_atox('1'), 1);
	ttest_check_eq(tlib_atox('2'), 2);
	ttest_check_eq(tlib_atox('3'), 3);
	ttest_check_eq(tlib_atox('4'), 4);
	ttest_check_eq(tlib_atox('5'), 5);
	ttest_check_eq(tlib_atox('6'), 6);
	ttest_check_eq(tlib_atox('7'), 7);
	ttest_check_eq(tlib_atox('8'), 8);
	ttest_check_eq(tlib_atox('9'), 9);

	ttest_check_eq(tlib_atox('A'), 0xA);
	ttest_check_eq(tlib_atox('B'), 0xB);
	ttest_check_eq(tlib_atox('C'), 0xC);
	ttest_check_eq(tlib_atox('D'), 0xD);
	ttest_check_eq(tlib_atox('E'), 0xE);
	ttest_check_eq(tlib_atox('F'), 0xF);

	ttest_check_eq(tlib_atox('a'), 0xA);
	ttest_check_eq(tlib_atox('b'), 0xB);
	ttest_check_eq(tlib_atox('c'), 0xC);
	ttest_check_eq(tlib_atox('d'), 0xD);
	ttest_check_eq(tlib_atox('e'), 0xE);
	ttest_check_eq(tlib_atox('f'), 0xF);

	for (ti c = 0; c <= UCHAR_MAX; c++)
	{
		if (!isxdigit(c))
		{
			ttest_check_eq(tlib_atox(c), -1);
		}
	}
}

ttest(test_tzero_tlib_xtoa, 0)
{
	ttest_check_eq(tlib_xtoa(0), '0');
	ttest_check_eq(tlib_xtoa(1), '1');
	ttest_check_eq(tlib_xtoa(2), '2');
	ttest_check_eq(tlib_xtoa(3), '3');
	ttest_check_eq(tlib_xtoa(4), '4');
	ttest_check_eq(tlib_xtoa(5), '5');
	ttest_check_eq(tlib_xtoa(6), '6');
	ttest_check_eq(tlib_xtoa(7), '7');
	ttest_check_eq(tlib_xtoa(8), '8');
	ttest_check_eq(tlib_xtoa(9), '9');

	ttest_check_eq(tlib_xtoa(0xA), 'A');
	ttest_check_eq(tlib_xtoa(0xB), 'B');
	ttest_check_eq(tlib_xtoa(0xC), 'C');
	ttest_check_eq(tlib_xtoa(0xD), 'D');
	ttest_check_eq(tlib_xtoa(0xE), 'E');
	ttest_check_eq(tlib_xtoa(0xF), 'F');

	ttest_check_eq(tlib_xtoa(-1), '\0');
}

ttest(test_tzero_tlib_byteToHex, 0)
{
	tb byte[64] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
	tc hex[64] = {0};

	ttest_check_eq(tlib_byteToHex(tnull, 1, tnull), -1);
	ttest_check_eq(tlib_byteToHex(byte, -1, tnull), -1);
	ttest_check_eq(tlib_byteToHex(byte, 10, tnull), 20);
	ttest_check_eq(tlib_byteToHex(byte, 10, hex), 20);
	ttest_check_str_eq(hex, "00010203040506070809");
}

ttest(test_tzero_tlib_hexToByte, 0)
{
	tc hex[64] = "00010203040506070809";
	tb byte[64] = {0};

	ttest_check_eq(tlib_hexToByte(tnull, 1, tnull), -1);
	ttest_check_eq(tlib_hexToByte(hex, -1, tnull), -1);
	ttest_check_eq(tlib_hexToByte(hex, 1, tnull), -1);
	ttest_check_eq(tlib_hexToByte(hex, 20, tnull), 10);
	ttest_check_eq(tlib_hexToByte(hex, 20, byte), 10);
	ttest_check_bytes_eq(byte, "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09", 10);
}
