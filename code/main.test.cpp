/**
 * @brief 
 * 
 * @file main.test.cpp
 */
#include <tlib.h>

ttest_import(ttype_test);
ttest_import(tlog_test);
ttest_import(version_test);

ttest_export(main_test)
{
	ttest_run(ttype_test, 0);
	ttest_run(tlog_test, 0);
	ttest_run(version_test, 0);
}
