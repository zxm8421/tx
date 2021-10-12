# tlib/ttype/ttype.pri

HEADERS += \
	$${PWD}/ttype.h	\
	$${PWD}/tstring.h

SOURCES +=

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/ttype.test.c
}
