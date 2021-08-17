#include "stable.h"

#include <pthread.h>
#include <unistd.h>

#if defined(__MINGW64__) || defined(__MINGW32__)
#include <locale.h>
#include <windows.h>
#endif

#include <QApplication>

#define TLOG_LOCAL_FILTER TLOG_I
#include <tlog/tlog.h>
#ifndef NTEST
#include <ttest/ttest.h>
#endif

#include "version/version.h"
#include "ui/mainwindow.h"

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
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

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
