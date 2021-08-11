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
buildTime = "$(shell PowerShell (Get-Date -UFormat %s).Split('.')[0])"
buildSalt = "$(shell PowerShell Get-Date -Format %ffffff)"
} else {
buildTime = "$(shell date +%s)"
buildSalt = "$(shell echo \$\$((\$\$(date +%N)/1000)))"
}

buildSHA1 = "$(shell git --git-dir $${PWD}/.git rev-parse --short=7 HEAD)"

DEFINES += \
	buildVer_Major=$${buildVer_Major}	\
	buildVer_Minor=$${buildVer_Minor}	\
	buildVer_Patch=$${buildVer_Patch}	\
	buildTime=$${buildTime}	\
	buildSalt=$${buildSalt}	\
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
message(release)
DEFINES += NDEBUG
} else {
message(debug)
}

# 单元测试
# DEFINES += NTEST
!contains(DEFINES, NTEST) {
message(进行单元测试)
QT += testlib

SOURCES +=	\

HEADERS += \
	
}

contains(QMAKE_HOST.os, Windows) {
buildVer.commands = "DEL /F $${OUT_PWD}/version.o"
} else {
buildVer.commands = "rm -f $${OUT_PWD}/version.o"
}
QMAKE_EXTRA_TARGETS += buildVer
PRE_TARGETDEPS += buildVer

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
