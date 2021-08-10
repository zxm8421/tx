#include "stable.h"

#include <QApplication>

#include "version/version.h"
#include "ui/mainwindow.h"

#define TLOG_TAG "main"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>

#include <pthread.h>
#include <unistd.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <locale.h>
#include <windows.h>
#endif

void *task(void *arg)
{
	ti i = *((int *)arg);
	ti64 cnt = 0;
	while (true)
	{
		cnt = cnt + 1;
		tlog(TLOG_D, "task arg = %d, tid = %lu, cnt = %lld", i, pthread_self(), cnt);
	}

	return NULL;
}

#ifndef NDEBUG
int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
#if defined(__MINGW64__) || defined(__MINGW32__)
	tlog_system((tc *)"chcp 65001");
	setlocale(LC_ALL, ".65001");

	tlog_system((tc *)"echo Chinese中文");
#endif
	tlog(TLOG_I, "start test 开始测试...");
	showVer();

	return 0;
}
#else
int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
#if defined(__MINGW64__) || defined(__MINGW32__)
	// ::ShowWindow(::GetConsoleWindow(), SW_NORMAL);
	tlog_system((tc *)"chcp 65001");
	setlocale(LC_ALL, ".65001");
	// ::FreeConsole();

	tlog_system((tc *)"echo Chinese中文");
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

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
#endif
