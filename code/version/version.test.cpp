#include "version.h"
#include "stable.h"

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_static(version_test_showVer)
{
	ttest_check_eq(showVer(), 0);
}

ttest_export(version_test)
{
	ttest_run(version_test_showVer, 200);
}
