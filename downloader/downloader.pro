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
MOC_DIR = build/moc

INCLUDEPATH += .
INCLUDEPATH += src

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../logger/src
LIBS += -L../logger/bin -llogger
INCLUDEPATH += ../settings/src
LIBS += -L../settings/bin -lsettings

FORMS += ui/authenticationdialog.ui
FORMS += ui/mainwindow.ui

SOURCES += src/main.cpp
SOURCES += src/config/global.cpp
SOURCES += src/downloader/appdownloader.cpp
SOURCES += src/fs/apptreemanager.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/network/downloadmanager.cpp
SOURCES += src/utils/hashmac/hashmac512.cpp
SOURCES += src/xml/data/application.cpp
SOURCES += src/xml/data/download.cpp
SOURCES += src/xml/data/javaupdate.cpp
SOURCES += src/xml/deploymentxml.cpp

HEADERS += src/config/global.h
HEADERS += src/downloader/appdownloader.h
HEADERS += src/fs/apptreemanager.h
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
