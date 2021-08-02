#include "stable.h"

#include <QApplication>

#include "version/version.h"
#include "ui/mainwindow.h"

#define TLOG_TAG "main"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>

int main(int argc, char *argv[])
{
	tlog(TLOG_I, "start ...");
	showVer();

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
