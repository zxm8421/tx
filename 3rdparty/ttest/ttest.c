#include "ttest.h"
#include "stable.h"

#include <stdio.h>
#include <string.h>

#define TLOG_LOCAL_FILTER TLOG_I
#include <tlog/tlog.h>

ttest_import(main_test);
int ttest_main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	tlog(TLOG_T, "开始自动化测试");

	struct ttest_Ret _ret;
	struct ttest_Ret *ret = &_ret;
	memset(ret, 0, sizeof(struct ttest_Ret));
	ttest_run(main_test, 0);

	tlog(TLOG_T, "结束自动化测试");

	return ret->failed;
}

ti ttest_run_test(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, void (*ttest_test)(struct ttest_Ret *ret), bool run, ti timeout)
{
	ret->check_passed = 0;
	ret->check_failed = 0;

	struct ttest_Ret _subret;
	struct ttest_Ret *subret = &_subret;
	memset(subret, 0, sizeof(struct ttest_Ret));

	tc buf[32] = {0};

	ti ms = 0;
	if (run)
	{
		ti64 start = tlog_getTimeMs();
		ttest_test(subret);
		ti64 end = tlog_getTimeMs();
		ms = end - start;

		if ((subret->check_failed > 0) || (subret->failed > 0) || ((timeout > 0) && (ms > timeout)))
		{
			subret->failed += 1;
			snprintf(buf, sizeof(buf), "test failed");
		}
		else
		{
			subret->passed += 1;
			snprintf(buf, sizeof(buf), "test passed");
		}
	}
	else
	{
		subret->skipped += 1;
		snprintf(buf, sizeof(buf), "test skipped");
	}

	subret->sum += 1;

	ret->passed += subret->passed;
	ret->failed += subret->failed;
	ret->skipped += subret->skipped;
	ret->sum += subret->sum;

	tlog_rawprint(file, line, func, filter, TLOG_T,
				  "%s\n"
				  "%8d ms  sum  passed  failed skipped\n"
				  "this      %6d  %6d  %6d  %6d\n"
				  "all       %6d  %6d  %6d  %6d",
				  buf,
				  ms,
				  subret->sum, subret->passed, subret->failed, subret->skipped,
				  ret->sum, ret->passed, ret->failed, ret->skipped);

	return subret->failed;
}

ti ttest_rawcheck(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, bool v)
{
	if (v)
	{
		ret->check_passed += 1;
		// tlog_rawprint(file, line, func, filter, TLOG_T, "check passed");
		return 0;
	}
	else
	{
		ret->check_failed += 1;
		tlog_rawprint(file, line, func, filter, TLOG_T, "check failed");

		return 1;
	}
}