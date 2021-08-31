# tlib/tsh/tsh.pri

HEADERS += \
	$${PWD}/tsh.conf.h	\
	$${PWD}/tsh.h	\
	$${PWD}/tsh.in.h

SOURCES +=	\
	$${PWD}/tsh.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/tsh.test.c
}
