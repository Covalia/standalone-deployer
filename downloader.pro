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
    ui/welcome.cpp \
    ui/windows.cpp \
    ui/personnalize.cpp

HEADERS += src/mainwindow.h \
    src/log/simpleqtlogger.h \
    src/log/logger.h \
    src/xml/deploymentxml.h \
    src/xml/download.h \
    src/app/application.h \
    src/app/javaupdate.h \
        src/shortcut/shortcut.h \
    src/style/stylemanager.h \
    ui/welcome.h \
    ui/windows.h \
    ui/personnalize.h

FORMS += ui/mainwindow.ui \
    ui/welcome.ui \
    ui/windows.ui \
    ui/personnalize.ui

RESOURCES += resources.qrc

DISTFILES += templateFormatCode.cfg

TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts

