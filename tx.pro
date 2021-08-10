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

# message(VERSION = $${VERSION})

contains(QMAKE_HOST.os, Linux) {
buildTime = $$system("date +%s")
} else: contains(QMAKE_HOST.os, Windows) {
buildTime = $$system("PowerShell (Get-Date -UFormat %s).Split('.')[0]")
} else {
buildTime = 0
}

buildSHA1 = $$system("git rev-parse --short=7 HEAD")

# message(buildTime = $${buildTime})
# message(buildSHA1 = $${buildSHA1})

DEFINES += \
	buildVer_Major=$${buildVer_Major}	\
	buildVer_Minor=$${buildVer_Minor}	\
	buildVer_Patch=$${buildVer_Patch}	\
	buildTime=$${buildTime}	\
	buildSHA1=\\\"$${buildSHA1}\\\"

# CONFIG += precompile_header
# PRECOMPILED_HEADER = app/stable.h

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
} else {
QT += testlib

SOURCES +=	\
	app/version/version_test.cpp
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
