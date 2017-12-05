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
SOURCES += src/utils/crypt/cryptmanager.cpp
SOURCES += src/log/simpleqtlogger.cpp
SOURCES += src/log/logger.cpp
SOURCES += src/xml/download.cpp
SOURCES += src/xml/deploymentxml.cpp
SOURCES += src/app/application.cpp
SOURCES += src/app/javaupdate.cpp
SOURCES += src/shortcut/shortcut.cpp
SOURCES += src/utils/hashmac/hashmac512.cpp
SOURCES += src/config/global.cpp
SOURCES += src/settings/settings.cpp
SOURCES += src/commandline/commandlineparser.cpp
SOURCES += src/style/stylemanager.cpp
SOURCES += src/gui/welcome.cpp
SOURCES += src/gui/window.cpp
SOURCES += src/gui/personalize.cpp
SOURCES += src/language/languagemanager.cpp
SOURCES += src/uimanager/uimanager.cpp
SOURCES += src/gui/proxy.cpp
SOURCES += src/gui/about.cpp
SOURCES += src/gui/downloadui.cpp
SOURCES += src/gui/askpopup.cpp
SOURCES += src/utils/crypt/simplecrypt.cpp

HEADERS += src/utils/crypt/cryptmanager.h
HEADERS += src/log/simpleqtlogger.h
HEADERS += src/log/logger.h
HEADERS += src/xml/deploymentxml.h
HEADERS += src/xml/download.h
HEADERS += src/app/application.h
HEADERS += src/app/javaupdate.h
HEADERS += src/shortcut/shortcut.h
HEADERS += src/utils/hashmac/hashmac512.h
HEADERS += src/config/global.h
HEADERS += src/settings/settings.h
HEADERS += src/commandline/commandlineparser.h
HEADERS += src/style/stylemanager.h
HEADERS += src/gui/welcome.h
HEADERS += src/gui/window.h
HEADERS += src/gui/personalize.h
HEADERS += src/language/languagemanager.h
HEADERS += src/uimanager/uimanager.h
HEADERS += src/gui/proxy.h
HEADERS += src/gui/about.h
HEADERS += src/gui/askpopup.h
HEADERS += src/gui/downloadui.h 
HEADERS += src/utils/crypt/simplecrypt.h

FORMS += ui/welcome.ui
FORMS += ui/window.ui
FORMS += ui/personalize.ui
FORMS += ui/proxy.ui
FORMS += ui/about.ui
FORMS += ui/downloadui.ui
FORMS += ui/askpopup.ui

RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts

