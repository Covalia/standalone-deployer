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
