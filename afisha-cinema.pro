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

win32-msvc|win32-msvc.net|win32-msvc2005|win32-msvc2008 {
	CONFIG += win32-msvc-flags
}

# Speed up compilation process
win32-msvc-flags:debug {
	# /MD (Multithreaded runtime)  http://msdn2.microsoft.com/en-us/library/2kzt1wy3.aspx
	# /Gm (Enable Minimal Rebuild) http://msdn2.microsoft.com/en-us/library/kfz8ad09.aspx
	# /INCREMENTAL                 http://msdn2.microsoft.com/en-us/library/4khtbfyf.aspx
	QMAKE_CFLAGS   += /Gm
	QMAKE_CXXFLAGS += /Gm
	QMAKE_LFLAGS += /INCREMENTAL
}
