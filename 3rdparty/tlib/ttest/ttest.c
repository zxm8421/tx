#include "ttest.h"

#if defined(__MINGW64__) || defined(__MINGW32__)
#define __USE_MINGW_ANSI_STDIO 1
#endif
#include <stdio.h>
#include <string.h>

__attribute__((weak)) ttest_export(test_main)
{
	tlog(TLOG_T, "请定义test_main");
	ttest_check_fail();
}

ttest_import(test_main);
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
	ttest_run(test_main, 0);

	tlog(TLOG_T, "结束自动化测试");

	return ret->test_failed;
}

ti ttest_run_test(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, void (*ttest_test)(struct ttest_Ret *ret), const tc *ttest_test_name, bool run, ti timeout)
{
	tlog_rawprint(file, line, func, filter, TLOG_T,
				  "测试开始 %s",
				  ttest_test_name);

	ret->check = 0;
	ret->check_passed = 0;
	ret->check_failed = 0;

	struct ttest_Ret _subret;
	struct ttest_Ret *subret = &_subret;
	memset(subret, 0, sizeof(struct ttest_Ret));

	tc buf[256] = {0};

	tf ms = 0;
	if (run)
	{
		ti64 watch = 0;
		tlib_watch(&watch);
		ttest_test(subret);
		ms = tlib_watch(&watch) * 1e3;

		if (((timeout > 0) && (ms > timeout)))
		{
			subret->test_failed += 1;
			snprintf(buf, sizeof(buf), "测试超时");
		}
		else if ((subret->check_failed > 0) || (subret->test_failed > 0))
		{
			subret->test_failed += 1;
			snprintf(buf, sizeof(buf), "测试失败");
		}
		else
		{
			subret->test_passed += 1;
			snprintf(buf, sizeof(buf), "测试通过");
		}
	}
	else
	{
		subret->test_skipped += 1;
		snprintf(buf, sizeof(buf), "测试跳过");
	}

	subret->test += 1;

	ret->test_passed += subret->test_passed;
	ret->test_failed += subret->test_failed;
	ret->test_skipped += subret->test_skipped;
	ret->test += subret->test;

	if (subret->check > 0)
	{
		tlog_rawprint(file, line, func, filter, TLOG_T,
					  "%s %s 核对: %d (%d 通过, %d 失败)\n"
					  "%8.0f ms test  passed  failed skipped\n"
					  "this      %6d  %6d  %6d  %6d\n"
					  "all       %6d  %6d  %6d  %6d",
					  buf, ttest_test_name, subret->check, subret->check_passed, subret->check_failed,
					  ms,
					  subret->test, subret->test_passed, subret->test_failed, subret->test_skipped,
					  ret->test, ret->test_passed, ret->test_failed, ret->test_skipped);
	}
	else
	{
		tlog_rawprint(file, line, func, filter, TLOG_T,
					  "%s %s\n"
					  "%8.0f ms test  passed  failed skipped\n"
					  "this      %6d  %6d  %6d  %6d\n"
					  "all       %6d  %6d  %6d  %6d",
					  buf, ttest_test_name,
					  ms,
					  subret->test, subret->test_passed, subret->test_failed, subret->test_skipped,
					  ret->test, ret->test_passed, ret->test_failed, ret->test_skipped);
	}

	return subret->test_failed;
}

bool ttest_rawcheck(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, bool v)
{
	if (v)
	{
		ret->check_passed += 1;
		// tlog_rawprint(file, line, func, filter, TLOG_T, "核对通过");
	}
	else
	{
		ret->check_failed += 1;
		tlog_rawprint(file, line, func, filter, TLOG_T, "核对失败");
	}

	ret->check += 1;

	return v;
}
