# this project is required by common-core and common-gui
# so it is build as staticlib to be used in both common projects.
QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

TARGET = logger
TEMPLATE = lib

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

SOURCES += src/log/logger.cpp
SOURCES += src/log/simpleqtlogger.cpp

HEADERS += src/log/logger.h
HEADERS += src/log/simpleqtlogger.h
