QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

TARGET = settings
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
