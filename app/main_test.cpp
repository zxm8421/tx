#include "stable.h"

#define TLOG_LOCAL_FILTER TLOG_I
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_import(version_test);
ttest_export(main_test)
{
	ttest_run(version_test, 0);
}
