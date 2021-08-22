#include "ttest.h"
#include "stable.h"

#include <stdio.h>
#include <string.h>

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>

ttest_import(main_test);
int ttest_main(int argc, char *argv[])
{
	tlog(TLOG_T, "开始自动化测试");

	tlog(TLOG_T, "argc = %d", argc);
	for (ti i = 0; i < argc; i++)
	{
		tlog(TLOG_T, "argv[%d] = %s", i, argv[i]);
	}

	struct ttest_Ret _ret;
	struct ttest_Ret *ret = &_ret;
	memset(ret, 0, sizeof(struct ttest_Ret));
	ttest_run(main_test, 0);

	tlog(TLOG_T, "结束自动化测试");

	return ret->failed;
}

ti ttest_run_test(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, void (*ttest_test)(struct ttest_Ret *ret), const tc *ttest_test_name, bool run, ti timeout)
{
	tlog_rawprint(file, line, func, filter, TLOG_T,
				  "测试开始 %s",
				  ttest_test_name);

	ret->check_passed = 0;
	ret->check_failed = 0;

	struct ttest_Ret _subret;
	struct ttest_Ret *subret = &_subret;
	memset(subret, 0, sizeof(struct ttest_Ret));

	tc buf[256] = {0};

	ti ms = 0;
	if (run)
	{
		ti64 start = tlog_getTimeMs();
		ttest_test(subret);
		ti64 end = tlog_getTimeMs();
		ms = end - start;

		if (((timeout > 0) && (ms > timeout)))
		{
			subret->failed += 1;
			snprintf(buf, sizeof(buf), "测试超时");
		}
		else if ((subret->check_failed > 0) || (subret->failed > 0))
		{
			subret->failed += 1;
			snprintf(buf, sizeof(buf), "测试失败");
		}
		else
		{
			subret->passed += 1;
			snprintf(buf, sizeof(buf), "测试通过");
		}
	}
	else
	{
		subret->skipped += 1;
		snprintf(buf, sizeof(buf), "测试跳过");
	}

	subret->sum += 1;

	ret->passed += subret->passed;
	ret->failed += subret->failed;
	ret->skipped += subret->skipped;
	ret->sum += subret->sum;

	tlog_rawprint(file, line, func, filter, TLOG_T,
				  "%s %s\n"
				  "%8d ms  sum  passed  failed skipped\n"
				  "this      %6d  %6d  %6d  %6d\n"
				  "all       %6d  %6d  %6d  %6d",
				  buf, ttest_test_name,
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
		// tlog_rawprint(file, line, func, filter, TLOG_T, "核对通过");
		return 0;
	}
	else
	{
		ret->check_failed += 1;
		tlog_rawprint(file, line, func, filter, TLOG_T, "核对失败");

		return 1;
	}
}