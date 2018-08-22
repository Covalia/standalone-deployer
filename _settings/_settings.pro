QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

CONFIG(release, debug|release) {
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = settings
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

SOURCES += src/lang/languagemanager.cpp
SOURCES += src/secret/keymanager.cpp
SOURCES += src/settings/resourcessettings.cpp
SOURCES += src/settings/settings.cpp
SOURCES += src/utils/crypt/cryptmanager.cpp
SOURCES += src/utils/crypt/qblowfish/qblowfish.cpp

HEADERS += src/lang/languagemanager.h
HEADERS += src/secret/keymanager.h
HEADERS += src/settings/resourcessettings.h
HEADERS += src/settings/settings.h
HEADERS += src/utils/crypt/cryptmanager.h
HEADERS += src/utils/crypt/qblowfish/qblowfish.h
HEADERS += src/utils/crypt/qblowfish/qblowfish_p.h

defined(KEYMANAGER_RESOURCES, var) {
    RESOURCES += $$KEYMANAGER_RESOURCES
    message("Using key manager resources from file: $$KEYMANAGER_RESOURCES")
}
else {
    RESOURCES += keymanager_resources.qrc
}

