QT += core
QT += gui
QT += widgets

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

TARGET = installer
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

FORMS += ui/about.ui
FORMS += ui/askpopup.ui
FORMS += ui/downloadui.ui
FORMS += ui/personalize.ui
FORMS += ui/proxy.ui
FORMS += ui/welcome.ui
FORMS += ui/window.ui

SOURCES += src/main.cpp
SOURCES += src/commandline/commandlineparser.cpp
SOURCES += src/gui/about.cpp
SOURCES += src/gui/askpopup.cpp
SOURCES += src/gui/downloadui.cpp
SOURCES += src/gui/personalize.cpp
SOURCES += src/gui/proxy.cpp
SOURCES += src/gui/welcome.cpp
SOURCES += src/gui/window.cpp
SOURCES += src/shortcut/shortcut.cpp
SOURCES += src/style/stylemanager.cpp
SOURCES += src/uimanager/uimanager.cpp

HEADERS += src/commandline/commandlineparser.h
HEADERS += src/gui/about.h
HEADERS += src/gui/askpopup.h
HEADERS += src/gui/downloadui.h
HEADERS += src/gui/personalize.h
HEADERS += src/gui/proxy.h
HEADERS += src/gui/welcome.h
HEADERS += src/gui/window.h
HEADERS += src/shortcut/shortcut.h
HEADERS += src/style/stylemanager.h
HEADERS += src/uimanager/uimanager.h

RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts
