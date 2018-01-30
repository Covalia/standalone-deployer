QT += core

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

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../_logger/src
LIBS += -L../_logger/bin -llogger

SOURCES += src/fs/apptreemanager.cpp
SOURCES += src/io/config.cpp
SOURCES += src/tools/factory/factory.cpp
SOURCES += src/tools/installpath/installpath.cpp
SOURCES += src/tools/installpath/installpathimpl.cpp
SOURCES += src/tools/shortcut/shortcut.cpp
SOURCES += src/tools/shortcut/shortcutimpl.cpp

HEADERS += src/fs/apptreemanager.h
HEADERS += src/io/config.h
HEADERS += src/tools/factory/factory.h
HEADERS += src/tools/installpath/installpath.h
HEADERS += src/tools/installpath/installpathimpl.h
HEADERS += src/tools/shortcut/shortcut.h
HEADERS += src/tools/shortcut/shortcutimpl.h

win32 {
HEADERS += src/tools/factory/windows/windowsfactory.h
HEADERS += src/tools/installpath/windows/windowsinstallpathimpl.h
HEADERS += src/tools/shortcut/windows/windowsshortcutimpl.h

SOURCES += src/tools/factory/windows/windowsfactory.cpp
SOURCES += src/tools/installpath/windows/windowsinstallpathimpl.cpp
SOURCES += src/tools/shortcut/windows/windowsshortcutimpl.cpp
}

macx {
HEADERS += src/tools/factory/macos/macosfactory.h
HEADERS += src/tools/installpath/macos/macosinstallpathimpl.h
HEADERS += src/tools/shortcut/macos/macosshortcutimpl.h

SOURCES += src/tools/factory/macos/macosfactory.cpp
SOURCES += src/tools/installpath/macos/macosinstallpathimpl.cpp
SOURCES += src/tools/shortcut/macos/macosshortcutimpl.cpp
}
