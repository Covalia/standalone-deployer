QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

TARGET = settings
TEMPLATE = lib

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../_logger/src
LIBS += -L../_logger/bin -llogger

SOURCES += src/lang/languagemanager.cpp
SOURCES += src/utils/crypt/cryptmanager.cpp
SOURCES += src/utils/crypt/simplecrypt.cpp
SOURCES += src/settings/resourcessettings.cpp
SOURCES += src/settings/settings.cpp

HEADERS += src/lang/languagemanager.h
HEADERS += src/utils/crypt/cryptmanager.h
HEADERS += src/utils/crypt/simplecrypt.h
HEADERS += src/settings/resourcessettings.h
HEADERS += src/settings/settings.h
