/**
 * @brief 
 * 
 * @file main.cpp
 */
#include <tlib.h>

#include <QApplication>

#include "version/version.h"
#include "ui/mainwindow.h"

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
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
