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

FORMS += ui/about.ui
FORMS += ui/askpopup.ui
FORMS += ui/download.ui
FORMS += ui/personalize.ui
FORMS += ui/proxy.ui
FORMS += ui/welcome.ui
FORMS += ui/window.ui

SOURCES += src/main.cpp
SOURCES += src/commandline/commandlineparser.cpp
SOURCES += src/gui/forms/aboutui.cpp
SOURCES += src/gui/forms/askpopupui.cpp
SOURCES += src/gui/forms/downloadui.cpp
SOURCES += src/gui/forms/personalizeui.cpp
SOURCES += src/gui/forms/proxyui.cpp
SOURCES += src/gui/forms/welcomeui.cpp
SOURCES += src/gui/forms/windowui.cpp
SOURCES += src/gui/manager/uimanager.cpp
SOURCES += src/gui/style/stylemanager.cpp
SOURCES += src/shortcut/shortcut.cpp

HEADERS += src/commandline/commandlineparser.h
HEADERS += src/gui/forms/aboutui.h
HEADERS += src/gui/forms/askpopupui.h
HEADERS += src/gui/forms/downloadui.h
HEADERS += src/gui/forms/personalizeui.h
HEADERS += src/gui/forms/proxyui.h
HEADERS += src/gui/forms/welcomeui.h
HEADERS += src/gui/forms/windowui.h
HEADERS += src/gui/manager/uimanager.h
HEADERS += src/gui/style/stylemanager.h
HEADERS += src/shortcut/shortcut.h

RESOURCES += fixed_resources.qrc

win32 {
RESOURCES += windows_resources.qrc
}

macx {
RESOURCES += macosx_resources.qrc
}

defined(OVERRIDABLE_INSTALLER_RESOURCES, var) {
    RESOURCES += $$OVERRIDABLE_INSTALLER_RESOURCES
    message("Using installer resources from file: $$OVERRIDABLE_INSTALLER_RESOURCES")
}
else {
    RESOURCES += overridable_resources.qrc
}

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts
