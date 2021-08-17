#include "version.h"
#include "stable.h"

#define TLOG_LOCAL_FILTER TLOG_I
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_static(showVer_test)
{
	ttest_check_eq(showVer(), 0);
}

ttest_export(version_test)
{
	ttest_run(showVer_test, 10);
}