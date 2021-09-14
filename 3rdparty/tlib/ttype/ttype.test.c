/**
 * @file ttype.test.c
 * 
 * @brief 
 * 
 */
#include "ttype.h"

#include <tlib.h>

ttest(test_ttype_os, 0)
{
#if defined(__MINGW64__)
	tlog(TLOG_T, "mingw64");
#elif defined(__MINGW32__)
	tlog(TLOG_T, "mingw32");
#elif defined(__linux__)
	tlog(TLOG_T, "linux");
#else
	tlog(TLOG_T, "other os");
#endif
	ttest_check_pass();
}

ttest(test_ttype_le_be, 0)
{
	union
	{
		int i;
		char s[sizeof(int)];
	} t;
	t.i = 1;

	if (t.s[0] == 1)
	{
		tlog(TLOG_T, "小端 Little Endian");
	}
	else
	{
		tlog(TLOG_T, "大端 Big Endian");
	}
	ttest_check_pass();
}

ttest(test_ttype_ctype, 0)
{
	tlog(TLOG_T, "__SIZEOF_POINTER__ = %d", __SIZEOF_POINTER__);
	tlog(TLOG_T, "sizeof(void *) = %lu", sizeof(void *));

	tlog(TLOG_T, "sizeof(char) = %lu", sizeof(char));
	tlog(TLOG_T, "sizeof(short) = %lu", sizeof(short));

	tlog(TLOG_T, "sizeof(int) = %lu", sizeof(int));
	tlog(TLOG_T, "sizeof(long) = %lu", sizeof(long));
	tlog(TLOG_T, "sizeof(long long) = %lu", sizeof(long long));

	tlog(TLOG_T, "sizeof(float) = %lu", sizeof(float));
	tlog(TLOG_T, "sizeof(double) = %lu", sizeof(double));
	tlog(TLOG_T, "sizeof(long double) = %lu", sizeof(long double));
	ttest_check_pass();
}

ttest(test_ttype_ttype, 0)
{
	ttest_check_eq(sizeof(tu8), 1);
	ttest_check_eq(sizeof(ti8), 1);
	ttest_check_eq(sizeof(tu16), 2);
	ttest_check_eq(sizeof(ti16), 2);
	ttest_check_eq(sizeof(tu32), 4);
	ttest_check_eq(sizeof(ti32), 4);
	ttest_check_eq(sizeof(tu64), 8);
	ttest_check_eq(sizeof(ti64), 8);
	ttest_check_true((sizeof(ti) == 4) || (sizeof(ti) == 8));
	ttest_check_true((sizeof(tisize) == 4) || (sizeof(tisize) == 8));
	ttest_check_true((sizeof(tusize) == 4) || (sizeof(tusize) == 8));

	ttest_check_eq(sizeof(tf32), 4);
	ttest_check_eq(sizeof(tf64), 8);
	ttest_check_true((sizeof(tf) == 4) || (sizeof(tf) == 8));

	ttest_check_eq(sizeof(tb8), 1);
	ttest_check_eq(sizeof(tb16), 2);
	ttest_check_eq(sizeof(tb32), 4);
	ttest_check_eq(sizeof(tb64), 8);
	ttest_check_true((sizeof(tbsize) == 4) || (sizeof(tbsize) == 8));
	ttest_check_eq(sizeof(tb), 1);

	ttest_check_eq(sizeof(tc8), 1);
	ttest_check_eq(sizeof(tc16), 2);
	ttest_check_eq(sizeof(tc32), 4);
	ttest_check_eq(sizeof(tc), 1);

	ttest_check_eq(tnull, NULL);
}

ttest(test_ttype_tx_array_size, 0)
{
	char a1[] = "1234";
	ttest_check_eq(tx_array_size(a1), 5);

	int a2[] = {0, 1, 2, 3, 4};
	ttest_check_eq(tx_array_size(a2), 5);

	double a3[] = {0, 1, 2, 3, 4};
	ttest_check_eq(tx_array_size(a3), 5);
}
