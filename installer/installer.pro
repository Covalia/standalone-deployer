QT += core
QT += gui
QT += widgets

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

TARGET = installer
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

SHARED_FOLDER_GUI = ../commongui
include($$SHARED_FOLDER_GUI/commongui.pri)
SHARED_FOLDER_CORE = ../commoncore
include($$SHARED_FOLDER_CORE/commoncore.pri)

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

SOURCES += src/main.cpp
SOURCES += src/commandline/commandlineparser.cpp
SOURCES += src/shortcut/shortcut.cpp

HEADERS += src/commandline/commandlineparser.h
HEADERS += src/shortcut/shortcut.h

# FORMS += ui/

RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts
