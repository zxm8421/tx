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

DEFINES += NTEST
!contains(DEFINES, NTEST) {
	QT += testlib
	CONFIG += testcase
}


buildVer_Major = 0
buildVer_Minor = 1
buildVer_Patch = 0
VERSION = $${buildVer_Major}.$${buildVer_Minor}.$${buildVer_Patch}

contains(QMAKE_HOST.os, Windows) {
buildTime = "$(shell PowerShell (Get-Date (Get-Date).ToUniversalTime() -UFormat %s).Split(\\\'.\\\')[0])"
buildSalt = "$(shell PowerShell Get-Date -Format 1%ffffff)"

} else {
buildTime = "$(shell date +%s)"
buildSalt = "$(shell date +1%6N)"

}

buildBranch = "$(shell git --git-dir $${PWD}/.git rev-parse --abbrev-ref HEAD)"
buildSHA1 = "$(shell git --git-dir $${PWD}/.git rev-parse --short=7 HEAD)"

DEFINES += \
	buildTarget=\\\"$${TARGET}\\\"	\
	buildVer_Major=$${buildVer_Major}	\
	buildVer_Minor=$${buildVer_Minor}	\
	buildVer_Patch=$${buildVer_Patch}	\
	buildTime=$${buildTime}	\
	buildSalt=$${buildSalt}	\
	buildBranch=\\\"$${buildBranch}\\\"	\
	buildSHA1=\\\"$${buildSHA1}\\\"

contains(QMAKE_HOST.os, Windows) {
	buildVer.commands = "PowerShell if (Test-Path $${OUT_PWD}/release/version.o) { (ls $${OUT_PWD}/release/version.o).LastWriteTimeUtc = Get-Date -Date \"2000/01/01\" };	\
									if (Test-Path $${OUT_PWD}/debug/version.o) { (ls $${OUT_PWD}/debug/version.o).LastWriteTimeUtc = Get-Date -Date \"2000/01/01\" }"
} else {
	buildVer.commands = "touch -t 200001010000.00 version.o"
}
QMAKE_EXTRA_TARGETS += buildVer
PRE_TARGETDEPS += buildVer


include(3rdparty/tlib.pri)


INCLUDEPATH +=	\
	code

HEADERS +=	\
	code/stable.h	\
	code/ui/mainwindow.h	\
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
