/**
 * @brief 
 * 
 * @file tzero.test.c
 */
#include "tzero.h"

#include <ctype.h>
#include <limits.h>

#include <unistd.h>

ttest_static(test_tzero_tlib_getTime)
{
	ttest_check_gt(tlib_getTime(), 0);
	ttest_check_gt(tlib_getTime_ms(), 0);
	ttest_check_gt(tlib_getTime_us(), 0);
	ttest_check_gt(tlib_getTime_ns(), 0);
}

ttest_static(test_tzero_tlib_watch)
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

ttest_static(test_tzero_tlib_basename)
{
	ttest_check_null(tlib_basename(NULL));
	ttest_check_str_eq(tlib_basename("main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("/home/test/abc/main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("C:\\main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("C:\\桌面\\main.c"), "main.c");
	ttest_check_str_eq(tlib_basename("C:\\桌面\\123/abc/main.c"), "main.c");
}

ttest_static(test_tzero_tlib_system)
{
	ttest_check_eq(tlib_system("echo 0123456789"), 0);
}

ttest_static(test_tzero_tlib_hash_str)
{
	ttest_check_ne(tlib_hash_str(tnull), 0);
	ttest_check_ne(tlib_hash_str(""), 0);
	ttest_check_ne(tlib_hash_str("0123456789ABCDEF"), 0);
}

ttest_static(test_tzero_tlib_atox)
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

ttest_static(test_tzero_tlib_xtoa)
{
	ttest_tlib_check(tlib_xtoa(0), '0');
	ttest_tlib_check(tlib_xtoa(1), '1');
	ttest_tlib_check(tlib_xtoa(2), '2');
	ttest_tlib_check(tlib_xtoa(3), '3');
	ttest_tlib_check(tlib_xtoa(4), '4');
	ttest_tlib_check(tlib_xtoa(5), '5');
	ttest_tlib_check(tlib_xtoa(6), '6');
	ttest_tlib_check(tlib_xtoa(7), '7');
	ttest_tlib_check(tlib_xtoa(8), '8');
	ttest_tlib_check(tlib_xtoa(9), '9');
	ttest_tlib_check(tlib_xtoa(0xA), 'A');
	ttest_tlib_check(tlib_xtoa(0xB), 'B');
	ttest_tlib_check(tlib_xtoa(0xC), 'C');
	ttest_tlib_check(tlib_xtoa(0xD), 'D');
	ttest_tlib_check(tlib_xtoa(0xE), 'E');
	ttest_tlib_check(tlib_xtoa(0xF), 'F');

	ttest_tlib_check(tlib_xtoa(-1), '\0');
}

ttest_static(test_tzero_tlib_byteToHex)
{
}

ttest_static(test_tzero_tlib_hexToByte)
{
}

ttest_export(test_tzero)
{
	ttest_run(test_tzero_tlib_getTime, 10);
	ttest_run(test_tzero_tlib_watch, 1000 * 10);
	ttest_run(test_tzero_tlib_basename, 10);
	ttest_run(test_tzero_tlib_system, 1000);
	ttest_run(test_tzero_tlib_hash_str, 10);
	ttest_run(test_tzero_tlib_atox, 10);
	ttest_run(test_tzero_tlib_xtoa, 10);
	ttest_run(test_tzero_tlib_byteToHex, 10);
	ttest_run(test_tzero_tlib_hexToByte, 10);
}
