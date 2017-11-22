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

    
SOURCES += src/main.cpp \
	src/mainwindow.cpp \
    src/log/simpleqtlogger.cpp \
    src/log/logger.cpp \
    src/xml/download.cpp \
    src/xml/deploymentxml.cpp \
    src/app/application.cpp \
    src/app/javaupdate.cpp \
	src/shortcut/shortcut.cpp \
    src/style/stylemanager.cpp \
    src/gui/welcome.cpp \
    src/gui/windows.cpp \
    src/gui/personnalize.cpp \
    src/language/languagemanager.cpp \
    src/uiManager/uimanager.cpp \
    src/gui/proxy.cpp \
    src/gui/about.cpp \
    src/gui/downloadui.cpp

HEADERS += src/mainwindow.h \
    src/log/simpleqtlogger.h \
    src/log/logger.h \
    src/xml/deploymentxml.h \
    src/xml/download.h \
    src/app/application.h \
    src/app/javaupdate.h \
    src/shortcut/shortcut.h \
    src/style/stylemanager.h \
    src/gui/welcome.h \
    src/gui/windows.h \
    src/gui/personnalize.h \
    src/language/languagemanager.h \
    src/uiManager/uimanager.h \
    src/gui/proxy.h \
    src/gui/about.h \
    src/gui/downloadui.h

FORMS += ui/mainwindow.ui \
    ui/welcome.ui \
    ui/windows.ui \
    ui/personnalize.ui \
    ui/proxy.ui \
    ui/about.ui \
    ui/downloadui.ui

RESOURCES += resources.qrc

DISTFILES += templateFormatCode.cfg

TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts

