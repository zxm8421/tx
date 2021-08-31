# tlib/tlog/tlog.pri

HEADERS += \
	$${PWD}/tlog.conf.h	\
	$${PWD}/tlog.h	\
	$${PWD}/tlog.in.h

SOURCES +=	\
	$${PWD}/tlog.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/tlog.test.c
}
