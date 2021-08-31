# tlib/ttest/ttest.pri

!contains(DEFINES, NTEST) {
	HEADERS += \
		$${PWD}/ttest.conf.h	\
		$${PWD}/ttest.h	\
		$${PWD}/ttest.in.h

	SOURCES +=	\
		$${PWD}/ttest.c
}
