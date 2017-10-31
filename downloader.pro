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

SOURCES += src/main.cpp
SOURCES += src/mainwindow.cpp

HEADERS += src/mainwindow.h

FORMS += ui/mainwindow.ui

RESOURCES += resources.qrc

TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts

