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
INCLUDEPATH += ../_settings/src
LIBS += -L../_settings/bin -lsettings
INCLUDEPATH += ../_filesystem/src
LIBS += -L../_filesystem/bin -lfilesystem

FORMS += ui/authenticationdialog.ui
FORMS += ui/mainwindow.ui

SOURCES += src/main.cpp
SOURCES += src/downloader/appdownloader.cpp
SOURCES += src/downloader/config.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/network/downloadmanager.cpp
SOURCES += src/utils/hashmac/hashmac512.cpp
SOURCES += src/xml/data/application.cpp
SOURCES += src/xml/data/download.cpp
SOURCES += src/xml/data/javaupdate.cpp
SOURCES += src/xml/deploymentxml.cpp

HEADERS += src/downloader/appdownloader.h
HEADERS += src/downloader/config.h
HEADERS += src/gui/mainwindow.h
HEADERS += src/network/downloadmanager.h
HEADERS += src/utils/hashmac/hashmac512.h
HEADERS += src/xml/data/application.h
HEADERS += src/xml/data/download.h
HEADERS += src/xml/data/javaupdate.h
HEADERS += src/xml/deploymentxml.h

RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts
