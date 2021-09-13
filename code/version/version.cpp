/**
 * @file version.cpp
 * 
 * @brief 
 * 
 */
#include "version.h"

#include <tlib.h>

#include <QDateTime>
#include <QTimeZone>

// 由buildVer.py自动生成
#include "buildVer.h"

#ifndef buildTarget
#pragma message("编译器未生成buildTarget")
#define buildTarget "app"
#endif

#ifndef buildVer_Major
#define buildVer_Major 0
#endif
#ifndef buildVer_Minor
#define buildVer_Minor 0
#endif
#ifndef buildVer_Patch
#define buildVer_Patch 0
#endif

#define buildVer (buildVer_Major * 1000 * 1000 + buildVer_Minor * 1000 + buildVer_Patch)

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
	verInfo += QDateTime::fromSecsSinceEpoch(buildTime).toUTC().toString("yyyyMMdd.HHmmss(t) ");
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

QString getBuildTarget()
{
	return QString(buildTarget);
}

QVersionNumber getVersionNumber()
{
	return QVersionNumber(buildVer_Major, buildVer_Minor, buildVer_Patch);
}

QDateTime getBuildTime()
{
	return QDateTime::fromSecsSinceEpoch(buildTime);
}

QString getBuildSalt()
{
	return QString(buildSalt);
}

QString getBuildBranch()
{
	return QString(buildBranch);
}

QString getBuildSHA1()
{
	return QString(buildSHA1);
}
