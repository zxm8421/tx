/**
 * @brief 
 * 
 * @file version.test.cpp
 */
#include "version.h"

#include <tlib.h>

ttest_static(version_test_showVer)
{
	ttest_check_eq(showVer(), 0);
}

ttest_export(version_test)
{
	ttest_run(version_test_showVer, 200);
}
