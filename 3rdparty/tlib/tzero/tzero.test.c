/**
 * @brief 
 * 
 * @file tzero.test.c
 */
#include "tzero.h"

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

ttest_export(test_tzero)
{
	ttest_run(test_tzero_tlib_getTime, 10);
	ttest_run(test_tzero_tlib_watch, 1000 * 10);
	ttest_run(test_tzero_tlib_basename, 10);
	ttest_run(test_tzero_tlib_system, 1000);
	ttest_run(test_tzero_tlib_hash_str, 10);
}
