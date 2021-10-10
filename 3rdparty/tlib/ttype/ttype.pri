# tlib/ttype/ttype.pri

HEADERS += \
	$${PWD}/ttype.h	\
	$${PWD}/tstring.h

SOURCES +=	\
	$${PWD}/tstring.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/ttype.test.c	\
		$${PWD}/tstring.test.c
}
