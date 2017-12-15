QT += core

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

TARGET = loader
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../logger/src
LIBS += -L../logger/bin -llogger
INCLUDEPATH += ../settings/src
LIBS += -L../settings/bin -lsettings

SOURCES += src/main.cpp

# HEADERS += src/

# RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg