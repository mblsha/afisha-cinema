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

DEFINES += NO_XMPP_STANZA
HEADERS += \
	../psi/iris/include/iris/xmpp_xmlcommon.h
SOURCES += \
	../psi/iris/src/xmpp/xmpp-im/xmpp_xmlcommon.cpp
INCLUDEPATH += ../psi/iris/include/iris
DEPENDPATH  += ../psi/iris/include/iris

include(../psi/qa/oldtest/unittest.pri)
