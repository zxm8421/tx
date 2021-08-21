#include "version.h"
#include "stable.h"

#include <QDateTime>
#include <QTimeZone>

#define TLOG_LOCAL_FILTER TLOG_D
#include <tlog/tlog.h>

ti showVer()
{
	QString verInfo("\n");
	verInfo += QString("========================================\n");
	verInfo += QString("========================================\n");
	verInfo += QString("%1 v%2.%3.%4(%5 %6) (%7) by ♪紫小墨\n")
				   .arg(buildTarget)
				   .arg(buildVer_Major)
				   .arg(buildVer_Minor)
				   .arg(buildVer_Patch)
				   .arg(buildSHA1)
				   .arg(buildBranch)
				   .arg(buildSalt);
	verInfo += QDateTime::fromSecsSinceEpoch(buildTime).toString("yyyyMMdd.HHmmss(t) ");
#ifdef NDEBUG
	verInfo += QString("Release ");
#else
	verInfo += QString("Debug ");
#endif

#ifdef NTEST
	verInfo += QString("\n");
#else
	verInfo += QString("单元测试\n");
#endif

	verInfo += QString("========================================\n");
	verInfo += QString("========================================\n");

	tlog(TLOG_I, "%s", verInfo.toUtf8().data());

	return 0;
}
