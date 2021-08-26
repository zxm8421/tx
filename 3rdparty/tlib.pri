# tlib.pri

INCLUDEPATH += \
	3rdparty

HEADERS += \
	3rdparty/tjz/ttype.h	\
	3rdparty/tjz/ttype.inc.h	\
	3rdparty/tlog/tlog.h	\
	3rdparty/tlog/tlog.inc.h	\
	3rdparty/tlog/tlog.conf.h

SOURCES +=	\
	3rdparty/tlog/tlog.c

!contains(DEFINES, NTEST) {
	HEADERS += \
		3rdparty/ttest/ttest.h	\
		3rdparty/ttest/ttest.inc.h	

	SOURCES +=	\
		3rdparty/ttest/ttest.c	\
		3rdparty/tjz/ttype.test.c	\
		3rdparty/tlog/tlog.test.c
}
