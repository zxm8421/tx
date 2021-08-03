#include "stable.h"

#include <QApplication>

#include "version/version.h"
#include "ui/mainwindow.h"

#define TLOG_TAG "main"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>

#include <pthread.h>
#include <unistd.h>

void *task(void *arg)
{
	ti i = *((int *)arg);
	ti64 cnt = 0;
	while (true)
	{
		cnt = cnt + 1;
		tlog(TLOG_D, "task arg = %d, tid = %lu, cnt = %lld", i, pthread_self(), cnt);
		usleep(1000);
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	tlog(TLOG_I, "start ...");
	showVer();

	for (ti i = 0; i < argc; i++)
	{
		tlog(TLOG_D, "argv[%d] = %s", i, argv[i]);
	}

#if 0
	tlog(TLOG_D, "main pid = %d, tid = %lu", getpid(), pthread_self());
	for (ti i = 0; i < (2 * sysconf(_SC_NPROCESSORS_ONLN)); i++)
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
