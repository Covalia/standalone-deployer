QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

TARGET = filesystem
TEMPLATE = lib

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

SOURCES += src/fs/apptreemanager.cpp
SOURCES += src/fs/config.cpp

HEADERS += src/fs/apptreemanager.h
HEADERS += src/fs/config.h

