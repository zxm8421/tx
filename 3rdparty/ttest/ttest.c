#include "ttest.h"
#include "stable.h"

#define TLOG_TAG "ttest"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>

ttest_import(main_test);
ti ttest_main(int argc, char *argv[])
{
	struct ttest_Ret ret;
	struct ttest_Ret *ttest_ret = &ret;
	ttest_run(main_test, 0);

	return ttest_ret->failed;
}

ti ttest_run_test(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, struct ttest_Ret *ttest_ret, void (*ttest_test)(struct ttest_Ret *ttest_ret), ti enable, ti64 timeout, ...)
{
	return 0;
}
