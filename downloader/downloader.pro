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
SOURCES += src/app/application.cpp
SOURCES += src/app/javaupdate.cpp
SOURCES += src/config/global.cpp
SOURCES += src/utils/hashmac/hashmac512.cpp
SOURCES += src/xml/deploymentxml.cpp
SOURCES += src/xml/download.cpp
SOURCES += src/downloader/appdownloader.cpp
SOURCES += src/fs/apptreemanager.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/network/downloadmanager.cpp

HEADERS += src/app/application.h
HEADERS += src/app/javaupdate.h
HEADERS += src/config/global.h
HEADERS += src/utils/hashmac/hashmac512.h
HEADERS += src/xml/deploymentxml.h
HEADERS += src/xml/download.h
HEADERS += src/downloader/appdownloader.h
HEADERS += src/fs/apptreemanager.h
HEADERS += src/gui/mainwindow.h
HEADERS += src/network/downloadmanager.h

RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts
