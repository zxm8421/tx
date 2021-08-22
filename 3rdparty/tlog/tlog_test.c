#define TLOG_LOCAL_FILTER TLOG_D
#include "tlog.h"

#include <pthread.h>
#include <unistd.h>

#include <ttest/ttest.h>

ttest_static(tlog_test_tlog)
{
	ttest_check_gt(tlog(TLOG_D, ""), 0);
	ttest_check_gt(tlog(TLOG_D, "This is a test log"), 0);
	ttest_check_gt(tlog(TLOG_D, "这是一条测试消息"), 0);
}

void *tlog_test_tlog_qps_main(void *arg)
{
	ti cnt = *((int *)arg);

	for (ti i = 0; i < cnt; i++)
	{
		tlog(TLOG_D, "task arg = %d, tid = %lu, cnt = %d", i, pthread_self(), cnt);
	}

	return NULL;
}

ttest_static(tlog_test_tlog_qps)
{
	pthread_t tid[4] = {0};
	ti cnt = 10000;
	tlog(TLOG_D, "main pid = %d, tid = %lu", getpid(), pthread_self());
	ti64 start = tlog_getTimeUs();
	for (ti i = 0; i < tx_array_size(tid); i++)
	{
		pthread_create(&tid[i], NULL, tlog_test_tlog_qps_main, &cnt);
		tlog(TLOG_D, "create tid = %lu", tid);
	}

	for (ti i = 0; i < tx_array_size(tid); i++)
	{
		pthread_join(tid[i], NULL);
	}
	ti64 end = tlog_getTimeUs();

	ti qps = 1e6 * cnt * tx_array_size(tid) / (end - start);
	tlog(TLOG_T, "qps = %d", qps);
	ttest_check_gt(qps, 0);
}

ttest_static(tlog_test_tlog_hexdump)
{
	const tc hexTable[256 * 2 + 1] = "000102030405060708090A0B0C0D0E0F"
									 "101112131415161718191A1B1C1D1E1F"
									 "202122232425262728292A2B2C2D2E2F"
									 "303132333435363738393A3B3C3D3E3F"
									 "404142434445464748494A4B4C4D4E4F"
									 "505152535455565758595A5B5C5D5E5F"
									 "606162636465666768696A6B6C6D6E6F"
									 "707172737475767778797A7B7C7D7E7F"
									 "808182838485868788898A8B8C8D8E8F"
									 "909192939495969798999A9B9C9D9E9F"
									 "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
									 "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
									 "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
									 "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
									 "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
									 "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	ttest_check_gt(tlog_hexdump(TLOG_D, "info", hexTable, 0), 0);
	ttest_check_gt(tlog_hexdump(TLOG_D, "info", hexTable, sizeof(hexTable)), 0);
}

void *tlog_test_tlog_hexdump_qps_main(void *arg)
{
	ti cnt = *((int *)arg);

	const tc hexTable[256 * 2 + 1] = "000102030405060708090A0B0C0D0E0F"
									 "101112131415161718191A1B1C1D1E1F"
									 "202122232425262728292A2B2C2D2E2F"
									 "303132333435363738393A3B3C3D3E3F"
									 "404142434445464748494A4B4C4D4E4F"
									 "505152535455565758595A5B5C5D5E5F"
									 "606162636465666768696A6B6C6D6E6F"
									 "707172737475767778797A7B7C7D7E7F"
									 "808182838485868788898A8B8C8D8E8F"
									 "909192939495969798999A9B9C9D9E9F"
									 "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
									 "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
									 "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
									 "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
									 "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
									 "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

	for (ti i = 0; i < cnt; i++)
	{
		tlog_hexdump(TLOG_D, "info", hexTable, sizeof(hexTable));
	}

	return NULL;
}

ttest_static(tlog_test_tlog_hexdump_qps)
{
	pthread_t tid[4] = {0};
	ti cnt = 1000;
	tlog(TLOG_D, "main pid = %d, tid = %lu", getpid(), pthread_self());
	ti64 start = tlog_getTimeUs();
	for (ti i = 0; i < tx_array_size(tid); i++)
	{
		pthread_create(&tid[i], NULL, tlog_test_tlog_hexdump_qps_main, &cnt);
		tlog(TLOG_D, "create tid = %lu", tid);
	}

	for (ti i = 0; i < tx_array_size(tid); i++)
	{
		pthread_join(tid[i], NULL);
	}
	ti64 end = tlog_getTimeUs();

	ti qps = 1e6 * cnt * tx_array_size(tid) / (end - start);
	tlog(TLOG_T, "qps = %d", qps);
	ttest_check_gt(qps, 0);
}

ttest_static(tlog_test_tlog_basename)
{
	ttest_check_null(tlog_basename(NULL));
	ttest_check_not_null(tlog_basename("main.c"));
	ttest_check_not_null(tlog_basename("/home/test/abc/main.c"));
	ttest_check_not_null(tlog_basename("C:\\main.c"));
	ttest_check_not_null(tlog_basename("C:\\桌面\\main.c"));
	ttest_check_not_null(tlog_basename("C:\\桌面\\123/abc/main.c"));
}

ttest_export(tlog_test)
{
	ttest_run(tlog_test_tlog, 10);
	ttest_run(tlog_test_tlog_qps, 1000 * 2);

	ttest_run(tlog_test_tlog_hexdump, 10);
	ttest_run(tlog_test_tlog_hexdump_qps, 1000 * 2);

	ttest_run(tlog_test_tlog_basename, 10);
}