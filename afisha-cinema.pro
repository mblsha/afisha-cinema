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

include($$PWD/iris-chunks/iris-chunks.pri)

UNITTEST_PRI = $$PWD/../psi/qa/oldtest/unittest.pri
exists($$UNITTEST_PRI) {
	include($$UNITTEST_PRI)
}
else {
	OBJECTS_DIR = _build
	MOC_DIR = _build
	UI_DIR = _build
	RCC_DIR = _build
}
