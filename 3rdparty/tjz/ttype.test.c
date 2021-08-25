#include "ttype.inc.h"

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>
#include <ttest/ttest.h>

ttest_static(ttype_test_os)
{
#if defined(__MINGW64__)
	tlog(TLOG_T, "mingw64");
#elif defined(__MINGW32__)
	tlog(TLOG_T, "mingw32");
#elif defined(__linux__)
	tlog(TLOG_T, "linux");
#endif
	ttest_check_pass();
}

ttest_static(ttype_test_le_be)
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

ttest_static(ttype_test_ctype)
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

ttest_static(ttype_test_ttype)
{
	ttest_check_eq(sizeof(tu8), 1);
	ttest_check_eq(sizeof(ti8), 1);
	ttest_check_eq(sizeof(tu16), 2);
	ttest_check_eq(sizeof(ti16), 2);
	ttest_check_eq(sizeof(tu32), 4);
	ttest_check_eq(sizeof(ti32), 4);
	ttest_check_eq(sizeof(tu64), 8);
	ttest_check_eq(sizeof(ti64), 8);
#ifdef __SIZEOF_INT128__
	ttest_check_eq(sizeof(tu128), 16);
	ttest_check_eq(sizeof(ti128), 16);
#endif
	ttest_check_true((sizeof(ti) == 4) || (sizeof(ti) == 8));
	ttest_check_true((sizeof(tisize) == 4) || (sizeof(tisize) == 8));
	ttest_check_true((sizeof(tusize) == 4) || (sizeof(tusize) == 8));

	ttest_check_eq(sizeof(tf32), 4);
	ttest_check_eq(sizeof(tf64), 8);
#if __SIZEOF_LONG_DOUBLE__ == 16
	ttest_check_eq(sizeof(tf128), 16);
#elif __SIZEOF_LONG_DOUBLE__ == 12
	ttest_check_eq(sizeof(tf96), 12);
#else
#endif
	ttest_check_true((sizeof(tf) == 4) || (sizeof(tf) == 8));

	ttest_check_eq(sizeof(tb8), 1);
	ttest_check_eq(sizeof(tb16), 2);
	ttest_check_eq(sizeof(tb32), 4);
	ttest_check_eq(sizeof(tb64), 8);
	ttest_check_eq(sizeof(tb), 1);

	ttest_check_eq(sizeof(tc8), 1);
	ttest_check_eq(sizeof(tc16), 2);
	ttest_check_eq(sizeof(tc32), 4);
	ttest_check_eq(sizeof(tc64), 8);
	ttest_check_eq(sizeof(tc), 1);

	ttest_check_eq(tnull, NULL);
}

ttest_static(ttype_test_tx_array_size)
{
	char a1[] = "1234";
	ttest_check_eq(tx_array_size(a1), 5);

	int a2[] = {0, 1, 2, 3, 4};
	ttest_check_eq(tx_array_size(a2), 5);

	double a3[] = {0, 1, 2, 3, 4};
	ttest_check_eq(tx_array_size(a3), 5);
}

ttest_export(ttype_test)
{
	ttest_run(ttype_test_os, 10);
	ttest_run(ttype_test_le_be, 10);
	ttest_run(ttype_test_ctype, 10);
	ttest_run(ttype_test_ttype, 10);
	ttest_run(ttype_test_tx_array_size, 10);
}
