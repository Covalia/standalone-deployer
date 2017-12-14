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

INCLUDEPATH += ../logger/src
LIBS += -L../logger/bin -llogger

SOURCES += src/utils/crypt/cryptmanager.cpp
SOURCES += src/utils/crypt/simplecrypt.cpp
SOURCES += src/settings/settings.cpp

HEADERS += src/utils/crypt/cryptmanager.h
HEADERS += src/utils/crypt/simplecrypt.h
HEADERS += src/settings/settings.h
