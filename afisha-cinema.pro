TEMPLATE = app
CONFIG += qt
QT += gui network xml xmlpatterns webkit
RESOURCES = queries.qrc

OBJECTS_DIR = _build
MOC_DIR = _build
UI_DIR = _build
RCC_DIR = _build

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

HEADERS += $$PWD/iris-chunks/xmpp_xmlcommon.h
SOURCES += $$PWD/iris-chunks/xmpp_xmlcommon.cpp
INCLUDEPATH += $$PWD/iris-chunks
