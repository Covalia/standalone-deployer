QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

CONFIG(release, debug|release) {
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = filesystem
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

SOURCES += src/fs/apptreemanager.cpp
SOURCES += src/fs/config.cpp
SOURCES += src/tools/factory/factory.cpp
SOURCES += src/tools/path/path.cpp
SOURCES += src/tools/path/pathimpl.cpp

HEADERS += src/fs/apptreemanager.h
HEADERS += src/fs/config.h
HEADERS += src/tools/factory/factory.h
HEADERS += src/tools/path/path.h
HEADERS += src/tools/path/pathimpl.h

win32 {
HEADERS += src/tools/factory/windows/windowsfactory.h
HEADERS += src/tools/path/windows/windowspathimpl.h

SOURCES += src/tools/factory/windows/windowsfactory.cpp
SOURCES += src/tools/path/windows/windowspathimpl.cpp
}

macx {
HEADERS += src/tools/factory/macos/macosfactory.h
HEADERS += src/tools/path/macos/macospathimpl.h

SOURCES += src/tools/factory/macos/macosfactory.cpp
SOURCES += src/tools/path/macos/macospathimpl.cpp
}
