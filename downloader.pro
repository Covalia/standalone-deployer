macx {
QMAKE_MAC_SDK = macosx10.13
}

QT += core
QT += gui
QT += widgets
QT += network

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

TARGET = downloader
TEMPLATE = app

DESTDIR = bin
OBJECTS_DIR = build/objects
RCC_DIR = build/rcc
UI_DIR = build/ui

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

SOURCES += src/main.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/network/downloadmanager.cpp
SOURCES += src/downloader/appdownloader.cpp
SOURCES += src/config/global.cpp
SOURCES += src/fs/apptreemanager.cpp
SOURCES += src/log/simpleqtlogger.cpp
SOURCES += src/log/logger.cpp
SOURCES += src/xml/download.cpp
SOURCES += src/xml/deploymentxml.cpp
SOURCES += src/app/application.cpp
SOURCES += src/app/javaupdate.cpp
SOURCES += src/shortcut/shortcut.cpp

HEADERS += src/gui/mainwindow.h
HEADERS += src/network/downloadmanager.h
HEADERS += src/downloader/appdownloader.h
HEADERS += src/config/global.h
HEADERS += src/fs/apptreemanager.h
HEADERS += src/log/simpleqtlogger.h
HEADERS += src/log/logger.h
HEADERS += src/xml/deploymentxml.h
HEADERS += src/xml/download.h
HEADERS += src/app/application.h
HEADERS += src/app/javaupdate.h
HEADERS += src/shortcut/shortcut.h

FORMS += ui/mainwindow.ui

RESOURCES += resources.qrc

DISTFILES += uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts

