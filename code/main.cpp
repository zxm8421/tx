#include "stable.h"

#include <QApplication>

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>
#ifndef NTEST
#include <ttest/ttest.h>
#endif

#include "tsh/tsh.h"

#include "version/version.h"
#include "ui/mainwindow.h"

TSH_CMD_DEFINE(tlog_test)
{
	printf("tsh test\n");
	printf("tsh 测试\n");

	return 0;
}
TSH_CMD_EXPORT(tlog_test, "tsh tlog 测试");

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
	tlog_init();

#ifndef NTEST
	return ttest_main(argc, argv);
#endif

	tlog(TLOG_I, "start ...");
	showVer();

	tsh_init();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
