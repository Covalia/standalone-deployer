QT += core
QT += gui
QT += widgets

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += app_bundle

CONFIG(debug, release|debug) {
	CONFIG += console
}

CONFIG(release, debug|release) {
	CONFIG -= console
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = uninstaller
TEMPLATE = app

# icone macosx
ICON = "$$TARGET".icns

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

# FORMS +=

SOURCES += src/main.cpp

# HEADERS += 

RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts
