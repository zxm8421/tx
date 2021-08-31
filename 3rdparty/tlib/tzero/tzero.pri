# tlib/tzero/tzero.pri

HEADERS += \
	$${PWD}/tzero.conf.h	\
	$${PWD}/tzero.h	\
	$${PWD}/tzero.in.h

SOURCES +=	\
	$${PWD}/tzero.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/tzero.test.c
}
