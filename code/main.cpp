/**
 * @brief 
 * 
 * @file main.cpp
 */
#if defined(__MINGW64__) || defined(__MINGW32__)
#include <windows.h>
#endif

#include <tlib.h>

#include <QApplication>

#include "version/version.h"
#include "ui/mainwindow.h"

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
#if defined(__MINGW64__) || defined(__MINGW32__)
	SetConsoleOutputCP(CP_UTF8);
	// FreeConsole();
#endif

	tlog_init();

#ifndef NTEST
	return ttest_main(argc, argv);
#endif

	tlog(TLOG_I, "start ...");
	showVer();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
