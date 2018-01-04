QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

TARGET = logger
TEMPLATE = lib

DESTDIR = bin

Release:OBJECTS_DIR = release/objects
Release:RCC_DIR = release/rcc
Release:UI_DIR = release/ui
Release:MOC_DIR = release/moc

Debug:OBJECTS_DIR = debug/objects
Debug:RCC_DIR = debug/rcc
Debug:UI_DIR = debug/ui
Debug:MOC_DIR = debug/moc

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

SOURCES += src/log/logger.cpp
SOURCES += src/log/simpleqtlogger.cpp

HEADERS += src/log/logger.h
HEADERS += src/log/simpleqtlogger.h
