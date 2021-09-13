# tlib/tlog/tlog.pri

HEADERS += \
	$${PWD}/tlog.h

SOURCES +=	\
	$${PWD}/tlog.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/tlog.test.c
}
