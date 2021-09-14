/**
 * @brief 
 * 
 * @file ttest.h
 */
#pragma once

#include <stdarg.h>
#include <string.h>

#include <ttype/ttype.h>

/*******************************************************************************
 * @par export
 */
struct ttest_Ret
{
	ti total;
	ti passed;
	ti failed;
};

typedef void (*ttest_Testcase)(struct ttest_Ret *pCheck);
struct ttest_Test
{
	const tc *file;
	const ti line;
	const ti filter;
	const tc *name;
	ttest_Testcase testcase;
	tf cost_limit;
	tf cost;
	struct ttest_Ret check;
};

#define ttest(test_, time)                                                                                                                \
	static void test_(struct ttest_Ret *pCheck);                                                                                          \
	static struct ttest_Test ttest_Test_##test_ = {                                                                                       \
		.file = __FILE__,                                                                                                                 \
		.line = __LINE__,                                                                                                                 \
		.filter = TLOG_LOCAL_FILTER,                                                                                                      \
		.name = #test_,                                                                                                                   \
		.testcase = &test_,                                                                                                               \
		.cost_limit = time,                                                                                                               \
		.cost = 0,                                                                                                                        \
		.check = {                                                                                                                        \
			.total = 0,                                                                                                                   \
			.passed = 0,                                                                                                                  \
			.failed = 0,                                                                                                                  \
		},                                                                                                                                \
	};                                                                                                                                    \
	__attribute__((section("ttest_Test_SymVTab"), aligned(sizeof(void *)))) struct ttest_Test *ttest_pTest_##test_ = &ttest_Test_##test_; \
	static void test_(struct ttest_Ret *pCheck)

#define ttest_check_exit return
#define ttest_check_pass() ttest_check(true)
#define ttest_check_fail() ttest_check(false)
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

#define ttest_check_str_eq(s1, s2) ttest_check(strcmp(s1, s2) == 0)
#define ttest_check_str_ne(s1, s2) ttest_check(strcmp(s1, s2) != 0)

#define ttest_check_bytes_eq(b1, b2, n) ttest_check(memcmp(b1, b2, n) == 0)
#define ttest_check_bytes_ne(b1, b2, n) ttest_check(memcmp(b1, b2, n) != 0)

/**
 * @brief 请勿直接使用，请使用ttest_check_xxx
 * 
 */
#define ttest_check(v) ttest_rawcheck(__FILE__, __LINE__, __func__, TLOG_LOCAL_FILTER, pCheck, v)

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief 自动化测试入口函数
	 * 在main中调用
	 * 
	 * @param argc 
	 * @param argv 
	 * @return 
	 */
	int ttest_main(int argc, char *argv[]);
#ifdef __cplusplus
}
#endif

/*******************************************************************************
 * @par impl
 */
#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 * @brief 请勿直接使用，请使用ttest_check_xxx
	 * 
	 * @param file 
	 * @param line 
	 * @param func 
	 * @param filter 
	 * @param ret 
	 * @param v 
	 * @return 
	 */
	tbo ttest_rawcheck(const tc *file, const ti line, const tc *func, const ti filter, struct ttest_Ret *pCheck, tbo v);

#ifdef __cplusplus
}
#endif
