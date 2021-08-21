QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

buildVer_Major = 0
buildVer_Minor = 1
buildVer_Patch = 0
VERSION = $${buildVer_Major}.$${buildVer_Minor}.$${buildVer_Patch}

contains(QMAKE_HOST.os, Windows) {
buildTime = "$(shell PowerShell (Get-Date (Get-Date).ToUniversalTime() -UFormat %s).Split(\\\'.\\\')[0])"
buildSalt = "$(shell PowerShell Get-Date -Format %ffffff)"
} else {
buildTime = "$(shell date +%s)"
buildSalt = "$(shell date +%6N)"
}

buildBranch = "$(shell git --git-dir $${PWD}/.git rev-parse --abbrev-ref HEAD)"
buildSHA1 = "$(shell git --git-dir $${PWD}/.git rev-parse --short=7 HEAD)"

DEFINES += \
	buildTarget=\\\"$${TARGET}\\\"	\
	buildVer_Major=$${buildVer_Major}	\
	buildVer_Minor=$${buildVer_Minor}	\
	buildVer_Patch=$${buildVer_Patch}	\
	buildTime=$${buildTime}	\
	buildSalt=\\\"$${buildSalt}\\\"	\
	buildBranch=\\\"$${buildBranch}\\\"	\
	buildSHA1=\\\"$${buildSHA1}\\\"

INCLUDEPATH +=	\
	app	\
	3rdparty

SOURCES +=	\
	3rdparty/tlog/tlog.c	\
	app/main.cpp	\
	app/ui/mainwindow.cpp	\
	app/version/version.cpp

HEADERS +=	\
	3rdparty/tjz/ttype.h	\
	3rdparty/tlog/tlog.h	\
	app/stable.h	\
	app/ui/mainwindow.h	\
	app/version/version.h

DISTFILES +=	\
	LICENSE.md	\
	README.md

CONFIG(release, debug | release) {
DEFINES += NDEBUG
}

# 单元测试
# DEFINES += NTEST
!contains(DEFINES, NTEST) {
message(进行单元测试)
QT += testlib

SOURCES +=	\
	3rdparty/ttest/ttest.c	\
	3rdparty/tjz/ttype_test.c	\
	3rdparty/tlog/tlog_test.c	\
	app/main_test.cpp	\
	app/version/version_test.cpp

HEADERS += \
	3rdparty/ttest/ttest.h
	
}

contains(QMAKE_HOST.os, Windows) {
buildVer.commands = "PowerShell if (Test-Path $${OUT_PWD}/release/version.o) { (ls $${OUT_PWD}/release/version.o).LastWriteTimeUtc = Get-Date -Date \"2000/01/01\" };	\
								if (Test-Path $${OUT_PWD}/debug/version.o) { (ls $${OUT_PWD}/debug/version.o).LastWriteTimeUtc = Get-Date -Date \"2000/01/01\" }"
} else {
buildVer.commands = "touch -t 200001010000.00 version.o"
}
QMAKE_EXTRA_TARGETS += buildVer
PRE_TARGETDEPS += buildVer

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
