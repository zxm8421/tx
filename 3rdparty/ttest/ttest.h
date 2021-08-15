/**
 * @brief 
 * 
 * @file ttest.h
 */
#pragma once

#include <stdarg.h>
#include <tjz/ttype.h>

struct ttest_Ret
{
	ti totol;
	ti passed;
	ti failed;
	ti skipped;
};

#define ttest_check_true(v) ttest_check_pass(struct ttest_Ret *ttest_ret, ti v);
#define ttest_check_false(v)
#define ttest_check_nothing(v)

#define ttest_check_null(v)
#define ttest_check_not_null(v)
// =
#define ttest_check_ti_eq(v1, v2)
// !=
#define ttest_check_ti_ne(v1, v2)
// <
#define ttest_check_ti_lt(v1, v2)
// >
#define ttest_check_ti_gt(v1, v2)
// <=
#define ttest_check_ti_le(v1, v2)
// >=
#define ttest_check_ti_ge(v1, v2)

#define ttest_check_

#define ttest_check_str_eq(s1, s2)
#define ttest_check_str_ne(s1, s2)

#define ttest_static(ttest_test) tstatic void ttest_test(struct ttest_Ret *ttest_ret)
#define ttest_export(ttest_test) textern void ttest_test(struct ttest_Ret *ttest_ret)
#define ttest_import(ttest_test) textern void ttest_test(struct ttest_Ret *ttest_ret)

#define ttest_run(test, timeout, ...) ttest_run_test((const tc *)TLOG_TAG, (const ti)TLOG_LVL, (const ti)TLOG_T, (const tc *)__FILE__, (const ti)__LINE__, (const tc *)__PRETTY_FUNCTION__, ttest_ret, test, 1, timeout, ##__VA_ARGS__)
#define ttest_skip(test) ttest_run_test((const tc *)TLOG_TAG, (const ti)TLOG_LVL, (const ti)TLOG_T, (const tc *)__FILE__, (const ti)__LINE__, (const tc *)__PRETTY_FUNCTION__, ttest_ret, test, 0, timeout)

#ifdef __cplusplus
extern "C"
{
#endif
	ti ttest_main(int argc, char *argv[]);

	ti ttest_run_test(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, struct ttest_Ret *ttest_ret, void (*ttest_test)(struct ttest_Ret *ttest_ret), ti enable, ti64 timeout, ...);

	ti ttest_check_pass(const tc *tag, const ti tag_level, const ti level, const tc *file, const ti line, const tc *func, struct ttest_Ret *ttest_ret, ti v);

#ifdef __cplusplus
}
#endif
