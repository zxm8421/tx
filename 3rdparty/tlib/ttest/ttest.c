/**
 * @file ttest.c
 * 
 * @brief 
 * 
 */
#include "ttest.h"

#if defined(__MINGW64__) || defined(__MINGW32__)
#define __USE_MINGW_ANSI_STDIO 1
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tlog/tlog.h>
#include <tzero/tzero.h>

static int ttest_Test_cmp(const void *a, const void *b)
{
	struct ttest_Test **itor_a = (struct ttest_Test **)a;
	struct ttest_Test **itor_b = (struct ttest_Test **)b;

	return strcmp((*itor_a)->name, (*itor_b)->name);
}

int ttest_main(int argc, char *argv[])
{
	tlog(TLOG_T, "开始自动化测试");

	for (ti i = 0; i < argc; i++)
	{
		tlog(TLOG_T, "argv[%d] = %s", i, argv[i]);
	}

	extern struct ttest_Test *__start_ttest_Test_SymVTab;
	extern struct ttest_Test *__stop_ttest_Test_SymVTab;

	qsort(&__start_ttest_Test_SymVTab,
		  &__stop_ttest_Test_SymVTab - &__start_ttest_Test_SymVTab, sizeof(struct ttest_Test *),
		  ttest_Test_cmp);

	struct ttest_Ret test;
	memset(&test, 0, sizeof(struct ttest_Ret));

	struct ttest_Ret check;
	memset(&check, 0, sizeof(struct ttest_Ret));
	tf cost = 0;

	for (struct ttest_Test **itor = &__start_ttest_Test_SymVTab; itor < &__stop_ttest_Test_SymVTab; itor++)
	{
		tc buf[256] = {0};

		ti64 watch = 0;
		tlib_watch(&watch);
		(*itor)->testcase(&(*itor)->check);
		(*itor)->cost = tlib_watch(&watch);

		if (((*itor)->cost_limit > 0) && ((*itor)->cost > (*itor)->cost_limit))
		{
			test.failed++;
			snprintf(buf, sizeof(buf), "测试超时");
		}
		else if ((*itor)->check.failed > 0)
		{
			test.failed++;
			snprintf(buf, sizeof(buf), "测试失败");
		}
		else
		{
			test.passed++;
			snprintf(buf, sizeof(buf), "测试通过");
		}
		cost += (*itor)->cost;
		test.total++;

		check.failed += (*itor)->check.failed;
		check.passed += (*itor)->check.passed;
		check.total += (*itor)->check.total;

		tlog_rawprint((*itor)->file, (*itor)->line, (*itor)->name, (*itor)->filter,
					  TLOG_T, "%s(%.03f) 核对: %d(= %d + %d)",
					  buf, (*itor)->cost, (*itor)->check.total, (*itor)->check.passed, (*itor)->check.failed);
	}

	tc buf[256] = {0};
	if (test.failed > 0)
	{
		snprintf(buf, sizeof(buf), "自动化测试失败");
	}
	else
	{
		snprintf(buf, sizeof(buf), "自动化测试通过");
	}
	tlog(TLOG_T, "(%.03f) 核对: %d(= %d + %d)", cost, check.total, check.passed, check.failed);
	tlog(TLOG_T, "%s 测试: %d(= %d + %d)", buf, test.total, test.passed, test.failed);

	return test.failed;
}

tbo ttest_rawcheck(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *check, tbo v)
{
	if (v)
	{
		check->passed += 1;
		// tlog_rawprint(file, line, func, filter, TLOG_T, "核对通过");
	}
	else
	{
		check->failed += 1;
		tlog_rawprint(file, line, func, filter, TLOG_T, "核对失败");
	}

	check->total += 1;

	return v;
}
