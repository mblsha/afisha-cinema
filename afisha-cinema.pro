TEMPLATE = app
CONFIG += qt
QT += gui network xml xmlpatterns webkit
RESOURCES = queries.qrc

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
	yamap.cpp \
	cinemaprogress.cpp

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
	yamap.h \
	cinemaprogress.h

include($$PWD/iris-chunks/iris-chunks.pri)
include($$PWD/unittest.pri)
