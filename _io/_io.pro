QT += core
QT += concurrent

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

CONFIG(release, debug|release) {
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = io
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
INCLUDEPATH += headers/libarchive

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../_logger/src

SOURCES += src/io/config.cpp
SOURCES += src/io/fileutils.cpp
SOURCES += src/io/unzip/zipextractor.cpp
SOURCES += src/factories/factory/factory.cpp
SOURCES += src/factories/apppath/apppath.cpp
SOURCES += src/factories/apppath/apppathimpl.cpp
SOURCES += src/factories/shortcut/shortcut.cpp
SOURCES += src/factories/shortcut/shortcutimpl.cpp

HEADERS += src/io/config.h
HEADERS += src/io/fileutils.h
HEADERS += src/io/unzip/qarchive/qarchive.h
HEADERS += src/io/unzip/zipextractor.h
HEADERS += src/factories/factory/factory.h
HEADERS += src/factories/apppath/apppath.h
HEADERS += src/factories/apppath/apppathimpl.h
HEADERS += src/factories/shortcut/shortcut.h
HEADERS += src/factories/shortcut/shortcutimpl.h

win32 {
	HEADERS += src/factories/factory/windows/windowsfactory.h
	HEADERS += src/factories/apppath/windows/windowsapppathimpl.h
	HEADERS += src/factories/shortcut/windows/windowsshortcutimpl.h

	SOURCES += src/factories/factory/windows/windowsfactory.cpp
	SOURCES += src/factories/apppath/windows/windowsapppathimpl.cpp
	SOURCES += src/factories/shortcut/windows/windowsshortcutimpl.cpp
}

macx {
	HEADERS += src/factories/factory/macos/macosfactory.h
	HEADERS += src/factories/apppath/macos/macosapppathimpl.h
	HEADERS += src/factories/shortcut/macos/macosshortcutimpl.h

	SOURCES += src/factories/factory/macos/macosfactory.cpp
	SOURCES += src/factories/apppath/macos/macosapppathimpl.cpp
	SOURCES += src/factories/shortcut/macos/macosshortcutimpl.cpp
}
