/**
 * @file main.test.cpp
 * 
 * @brief 
 * 
 */
#include <tlib.h>

ttest_import(test_tlib);
ttest_import(test_version);

ttest_export(test_main)
{
	ttest_run(test_tlib, 0);
	ttest_run(test_version, 0);
}
