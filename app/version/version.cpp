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
		 "tx v%d.%d.%d(%s %s)(%06d) by ♪紫小墨\n"
		 "buildTime %s",
		 buildVer_Major, buildVer_Minor, buildVer_Patch, buildSHA1, buildBranch, buildSalt,
		 QDateTime::fromSecsSinceEpoch(buildTime).toString("yyyyMMdd.HHmmss(t)").toUtf8().data());
#ifdef NDEBUG
	tlog(TLOG_I, "Release");
#else
	tlog(TLOG_I, "Debug");
#endif

	tlog(TLOG_I, "========================================");
	tlog(TLOG_I, "========================================");

	return 0;
}
