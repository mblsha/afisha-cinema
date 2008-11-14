TEMPLATE = app
CONFIG += qt
QT += gui network xml xmlpatterns
RESOURCES = queries.qrc

SOURCES += \
	main.cpp \
	cinema.cpp \
	cinemalist.cpp \
	httphelpers.cpp

HEADERS += \
	cinema.h \
	cinemalist.h \
	httphelpers.h

include(../psi/qa/oldtest/unittest.pri)
