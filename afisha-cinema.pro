TEMPLATE = app
CONFIG += qt
QT += gui network xml xmlpatterns webkit
RESOURCES = queries.qrc

OBJECTS_DIR = _build
MOC_DIR = _build
UI_DIR = _build

INTERFACES += \
	mainwindow.ui

SOURCES += \
	main.cpp \
	cinema.cpp \
	cinemalist.cpp \
	httphelpers.cpp \
	afishahelpers.cpp \
	httprequest.cpp \
	movie.cpp \
	movies.cpp \
	mainwindow.cpp \
	yamapsapi.cpp \
	geocoder.cpp \
	yamap.cpp

HEADERS += \
	cinema.h \
	cinemalist.h \
	httphelpers.h \
	afishahelpers.h \
	httprequest.h \
	movie.h \
	movies.h \
	mainwindow.h \
	yamapsapi.h \
	geocoder.h \
	yamap.h

DEFINES += NO_XMPP_STANZA
HEADERS += \
	../psi/iris/include/iris/xmpp_xmlcommon.h
SOURCES += \
	../psi/iris/src/xmpp/xmpp-im/xmpp_xmlcommon.cpp
INCLUDEPATH += ../psi/iris/include/iris
DEPENDPATH  += ../psi/iris/include/iris

include(../psi/qa/oldtest/unittest.pri)
