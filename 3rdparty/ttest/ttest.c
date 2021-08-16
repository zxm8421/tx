#include "ttest.h"
#include "stable.h"

#include <stdio.h>
#include <string.h>

#define TLOG_LOCAL_FILTER TLOG_I
#include <tlog/tlog.h>

ttest_import(main_test);
int ttest_main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	tlog(TLOG_T, "开始单元测试");

	struct ttest_Ret _ret;
	struct ttest_Ret *ret = &_ret;
	memset(ret, 0, sizeof(struct ttest_Ret));
	return ttest_run(main_test, 0);
}

ti ttest_run_test(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, void (*ttest_test)(struct ttest_Ret *ret), bool run, ti64 timeout)
{
	ret->check_failed = 0;

	struct ttest_Ret _subret;
	struct ttest_Ret *subret = &_subret;
	memset(subret, 0, sizeof(struct ttest_Ret));

	tc buf[16] = {0};

	if (run)
	{
		ti64 start = tlog_getTimeUs();
		ttest_test(subret);
		ti64 end = tlog_getTimeUs();
		ti64 us = end - start;

		if ((subret->check_failed > 0) || (subret->failed > 0) || ((timeout > 0) && (us > timeout)))
		{
			subret->failed += 1;
			snprintf(buf, sizeof(buf), "run failed");
		}
		else
		{
			subret->passed += 1;
			snprintf(buf, sizeof(buf), "run passed");
		}
	}
	else
	{
		subret->skipped += 1;
		snprintf(buf, sizeof(buf), "run skipped");
	}

	subret->total += 1;

	ret->passed += subret->passed;
	ret->failed += subret->failed;
	ret->skipped += subret->skipped;
	ret->total += subret->total;

	tlog_rawprint(file, line, func, filter, TLOG_T,
				  "%s, tolal passed failed skipped = %d %d %d %d; %d %d %d %d;",
				  buf,
				  subret->total, subret->passed, subret->failed, subret->skipped,
				  ret->total, ret->passed, ret->failed, ret->skipped);

	return subret->failed;
}

ti ttest_rawcheck(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ttest_ret, bool v)
{
	if (v)
	{
		// tlog_rawprint(file, line, func, filter, TLOG_T, "check passed");
		return 0;
	}
	else
	{
		ttest_ret->check_failed += 1;
		tlog_rawprint(file, line, func, filter, TLOG_T, "check failed");

		return 1;
	}
}