#include "stable.h"

#define TLOG_TAG "main_test"
#define TLOG_LVL TLOG_I
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_import(version_test);
ttest_export(main_test)
{
	ttest_run(version_test, 0);
}
