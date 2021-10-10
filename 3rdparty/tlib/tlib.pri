# tlib/tlib.pri

INCLUDEPATH += \
	$${PWD}/

include($${PWD}/ttype/ttype.pri)
include($${PWD}/tlog/tlog.pri)
include($${PWD}/tio/tio.pri)
include($${PWD}/tzero/tzero.pri)

!contains(DEFINES, NTEST) {
	include($${PWD}/ttest/ttest.pri)
}

HEADERS += \
	$${PWD}/tlib.h

!contains(DEFINES, NTEST) {
	SOURCES +=	\

}
