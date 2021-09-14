/**
 * @file version.test.cpp
 * 
 * @brief 
 * 
 */
#include "version.h"

#include <tlib.h>

ttest(test_version_showVer, 0.2)
{
	ttest_check_eq(showVer(), 0);
}
