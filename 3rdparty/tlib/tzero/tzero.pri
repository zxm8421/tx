# tlib/tzero/tzero.pri

HEADERS += \
	$${PWD}/tzero.h

SOURCES +=	\
	$${PWD}/tzero.c

!contains(DEFINES, NTEST) {
	SOURCES +=	\
		$${PWD}/tzero.test.c
}
