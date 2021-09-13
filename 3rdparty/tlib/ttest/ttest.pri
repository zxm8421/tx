# tlib/ttest/ttest.pri

!contains(DEFINES, NTEST) {
	HEADERS += \
		$${PWD}/ttest.h

	SOURCES +=	\
		$${PWD}/ttest.c
}
