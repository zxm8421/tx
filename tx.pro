QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# win32: CONFIG += console

CONFIG(release, debug|release) {
	DEFINES += NDEBUG
}

!equals(APP_TEST, 1) {
	DEFINES += NTEST
}

!contains(DEFINES, NTEST) {
	QT += testlib
	CONFIG += testcase
}

buildVer_Major = 0
buildVer_Minor = 1
buildVer_Patch = 0
VERSION = $${buildVer_Major}.$${buildVer_Minor}.$${buildVer_Patch}

DEFINES += \
	buildTarget=\\\"$${TARGET}\\\"	\
	buildVer_Major=$${buildVer_Major}	\
	buildVer_Minor=$${buildVer_Minor}	\
	buildVer_Patch=$${buildVer_Patch}

contains(QMAKE_HOST.os, Windows) {
	buildVer.commands = python $${PWD}/code/version/buildVer.py
} else {
	buildVer.commands = python3 $${PWD}/code/version/buildVer.py
}
QMAKE_EXTRA_TARGETS += buildVer
PRE_TARGETDEPS += buildVer


include(3rdparty/tlib/tlib.pri)


INCLUDEPATH +=	\
	code/

HEADERS +=	\
	code/ui/mainwindow.h	\
	code/version/buildVer.h
	code/version/version.h

SOURCES +=	\
	code/main.cpp	\
	code/ui/mainwindow.cpp	\
	code/version/version.cpp

DISTFILES +=	\
	LICENSE.md	\
	README.md

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		code/main.test.cpp	\
		code/version/version.test.cpp
}
