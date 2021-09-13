/**
 * @file version.test.cpp
 * 
 * @brief 
 * 
 */
#include "version.h"

#include <tlib.h>

ttest_static(test_version_showVer)
{
	ttest_check_eq(showVer(), 0);
}

ttest_export(test_version)
{
	ttest_run(test_version_showVer, 200);
}
