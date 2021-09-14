# tlib/tio/tio.pri

HEADERS += \
	$${PWD}/tio.h

SOURCES +=	\
	$${PWD}/tio.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/tio.test.c
}
