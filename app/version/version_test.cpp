#include "version.h"
#include "stable.h"

#define TLOG_TAG "version_test"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_static(showVer_test)
{
	ttest_check_gt(showVer(), 0);
}

ttest_export(version_test)
{
	ttest_run(showVer_test, 10);
}