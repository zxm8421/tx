#include "stable.h"

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_import(tlog_test);
ttest_import(version_test);

ttest_export(main_test)
{
	ttest_run(tlog_test, 0);
	ttest_run(version_test, 0);
}
