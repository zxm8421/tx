#include "stable.h"

#include <pthread.h>
#include <unistd.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <locale.h>
#include <windows.h>
#endif

#include <QApplication>

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>
#ifndef NTEST
#include <ttest/ttest.h>
#endif

#include "version/version.h"
#include "ui/mainwindow.h"

void *task(void *arg)
{
	ti i = *((int *)arg);
	ti cnt = 0;

	while (true)
	{
		cnt++;
		tlog(TLOG_D, "task arg = %d, tid = %lu, cnt = %d", i, pthread_self(), cnt);
	}

	return NULL;
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	tlog_init();
#if defined(__MINGW64__) || defined(__MINGW32__)
	// ::ShowWindow(::GetConsoleWindow(), SW_NORMAL);
	tlog_system("chcp 65001");
	setlocale(LC_ALL, ".65001");
	// ::FreeConsole();

	tlog_system("echo Chinese中文");
#endif

#ifndef NTEST
	return ttest_main(argc, argv);
#endif

	tlog(TLOG_I, "start ...");
	showVer();

	for (ti i = 0; i < argc; i++)
	{
		tlog(TLOG_D, "argv[%d] = %s", i, argv[i]);
	}

#if 0
	tlog(TLOG_D, "main pid = %d, tid = %lu", getpid(), pthread_self());
	for (ti i = 0; i < 64; i++)
	{
		pthread_t tid = 0;
		pthread_create(&tid, NULL, task, &i);
		tlog(TLOG_D, "create tid = %lu", tid);
	}
#endif

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
		ti64 start = tlog_getTimeUs();
		for (ti i = 0; i < 10000; i++)
		{
			tlog_hexdump(TLOG_D, "info", hexTable, sizeof(hexTable));
		}
		ti64 end = tlog_getTimeUs();
		tlog(TLOG_D, "cost = %.06f s", (end - start) / 1e6);
	}

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
