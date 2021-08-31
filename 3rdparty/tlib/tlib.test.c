/**
 * @brief 
 * 
 * @file tlib.test.c
 */
#include "tlib.h"

ttest_import(test_ttype);
ttest_import(test_tlog);
ttest_import(test_tzero);

ttest_export(test_tlib)
{
	ttest_run(test_ttype, 0);
	ttest_run(test_tlog, 0);
	ttest_run(test_tzero, 0);
}
