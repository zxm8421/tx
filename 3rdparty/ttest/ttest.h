/**
 * @brief 
 * 
 * @file ttest.h
 */
#pragma once

#include <tjz/ttype.h>

#define TTEST_PASS 0
#define TTEST_FAIL 1

#define TTEST_CHECK_TRUE
#define TTEST_CHECK_FALSE
#define TTEST_CHECK_NULL
#define TTEST_CHECK_NOT_NULL

#define TTEST_NAME_SIZE 256

struct ttest_Ret
{
	ti totol;
	ti passed;
	ti failed;
	ti skipped;
};

struct ttest_Testcase
{
	ti (*test)(int test_argc __attribute__((unused)), char *test_argv[] __attribute__((unused)));

	tc name[TTEST_NAME_SIZE];
	ti (*init)(void);
	ti (*cleanup)(void);
	ti64 timeout;
};

#define ttest_run(ret, testcase, ...) ttest_run_test((const tc *)TLOG_TAG, (ti)TLOG_LVL, (ti)TLOG_T, (const tc *)__FILE__, (ti)__LINE__, (const tc *)__PRETTY_FUNCTION__, (const tc *)format, ##__VA_ARGS__)

ti ttest_run_test();