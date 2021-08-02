#include "version.h"
#include "stable.h"
#include <QDateTime>
#include <QTimeZone>

#define TLOG_TAG "version"
#define TLOG_LVL TLOG_D
#include <tlog/tlog.h>

ti showVer()
{
	tlog(TLOG_I, "========================================");
	tlog(TLOG_I, "========================================");
	tlog(TLOG_I,
		 "\n"
		 "tx v%d.%d.%d(%s) by ♪紫小墨\n"
		 "buildTime %s",
		 buildVer_Major, buildVer_Minor, buildVer_Patch, buildSHA1,
		 QDateTime::fromSecsSinceEpoch(buildTime).toString("yyyyMMdd.HHmmss t").toLocal8Bit().data());

	tlog(TLOG_I, "========================================");
	tlog(TLOG_I, "========================================");
	return 0;
}
