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
	ti sum;
	ti passed;
	ti failed;
	ti skipped;

	ti check_failed;
};

#define ttest_check_nothing(v) ttest_check(true)
#define ttest_check_true(v) ttest_check(v)
#define ttest_check_false(v) ttest_check(!v)

#define ttest_check_null(v) ttest_check((v) == tnull)
#define ttest_check_not_null(v) ttest_check((v) != tnull)
#define ttest_check_eq(v1, v2) ttest_check((v1) == (v2))
#define ttest_check_ne(v1, v2) ttest_check((v1) != (v2))
#define ttest_check_lt(v1, v2) ttest_check((v1) < (v2))
#define ttest_check_gt(v1, v2) ttest_check((v1) > (v2))
#define ttest_check_le(v1, v2) ttest_check((v1) <= (v2))
#define ttest_check_ge(v1, v2) ttest_check((v1) >= (v2))

#define ttest_check_in_range(v, a, b) ttest_check(((v) >= (a)) && ((v) <= (b)))
#define ttest_check_not_in_range(v, a, b) ttest_check(((v) < (a)) || ((v) > (b)))

#define ttest_check_str_eq(s1, s2)
#define ttest_check_str_ne(s1, s2)

#define ttest_check(v) ttest_rawcheck(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, ret, v)

#define ttest_static(ttest_test) tstatic void ttest_test(struct ttest_Ret *ret)
#define ttest_export(ttest_test) textern void ttest_test(struct ttest_Ret *ret)
#define ttest_import(ttest_test) textern void ttest_test(struct ttest_Ret *ret)

#define ttest_run(test, ms) ttest_run_test(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, ret, test, true, ms)
#define ttest_skip(test) ttest_run_test(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, ret, test, false, 0)

#ifdef __cplusplus
extern "C"
{
#endif
	int ttest_main(int argc, char *argv[]);

	ti ttest_run_test(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, void (*ttest_test)(struct ttest_Ret *ret), bool run, ti timeout);

	ti ttest_rawcheck(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *ret, bool v);

#ifdef __cplusplus
}
#endif
