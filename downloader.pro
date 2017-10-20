macx {
QMAKE_MAC_SDK = macosx10.13
}

QT += core
QT += gui
QT += widgets
QT += network

CONFIG += warn_on
CONFIG += debug_and_release

CONFIG(debug, release|debug) {
	CONFIG -= app_bundle
	CONFIG += console
}

CONFIG(release, debug|release) {
	CONFIG += app_bundle
	CONFIG -= console
}

TARGET = downloader
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

SOURCES += src/main.cpp \
    src/log/simpleqtlogger.cpp \
    src/log/logger.cpp
SOURCES += src/mainwindow.cpp

HEADERS += src/mainwindow.h \
    src/log/simpleqtlogger.h \
    src/log/logger.h

FORMS += ui/mainwindow.ui

RESOURCES += resources.qrc

