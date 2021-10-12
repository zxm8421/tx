/**
 * @file main.cpp
 * 
 * @brief 
 * 
 */
#ifdef __MINGW64__
#include <windows.h>
#endif

#include <QApplication>

#include <tlib.h>

#include "ui/mainwindow.h"
#include "version/version.h"

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
#ifdef __MINGW64__
	SetConsoleOutputCP(CP_UTF8);
	// FreeConsole();
#endif

	tlog_init();
	tlog(TLOG_I, "start ...");
	showVer();

#ifndef NTEST
	return ttest_main(argc, argv);
#endif

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
