# tlib/ttype/ttype.pri

HEADERS += \
	$${PWD}/ttype.conf.h	\
	$${PWD}/ttype.h	\
	$${PWD}/ttype.in.h

SOURCES +=	\


!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/ttype.test.c
}
