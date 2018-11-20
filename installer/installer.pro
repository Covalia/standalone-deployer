QT += core
QT += gui
QT += widgets

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += app_bundle
CONFIG += resources_big

CONFIG(debug, release|debug) {
	CONFIG += console
}

CONFIG(release, debug|release) {
	CONFIG -= console
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = installer
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

# attention, l'ordre est important.
INCLUDEPATH += ../_io/src
INCLUDEPATH += ../_settings/src
INCLUDEPATH += ../_logger/src
LIBS += -L../_io/bin -lio
LIBS += -L../_settings/bin -lsettings
LIBS += -L../_logger/bin -llogger

macx {
LIBS += -L./libs/libarchive/macosx -larchive
}
win32 {
# attention, l'ordre est important.
LIBS += -L./libs/libarchive/win32 -larchive -lbz2 -lxml2 -lz -llzma -llz4 -lnettle -llzo
}

FORMS += ui/askpopup.ui
FORMS += ui/endinstallation.ui
FORMS += ui/installation.ui

SOURCES += src/main.cpp
SOURCES += src/commandline/commandlineparser.cpp
SOURCES += src/gui/forms/askpopupui.cpp
SOURCES += src/gui/forms/endinstallationui.cpp
SOURCES += src/gui/forms/installationui.cpp
SOURCES += src/gui/forms/windowui.cpp
SOURCES += src/gui/wizard/installwizard.cpp
SOURCES += src/gui/wizard/pages/folders.cpp
SOURCES += src/gui/wizard/pages/launch.cpp
SOURCES += src/gui/wizard/pages/license.cpp
SOURCES += src/gui/wizard/pages/proxy.cpp
SOURCES += src/gui/wizard/pages/shortcuts.cpp
SOURCES += src/gui/wizard/pages/type.cpp
SOURCES += src/gui/wizard/pages/welcome.cpp
SOURCES += src/gui/manager/uimanager.cpp
SOURCES += src/manager/installmanager.cpp
SOURCES += src/settings/resourcesettings.cpp
SOURCES += src/utils.cpp

HEADERS += src/commandline/commandlineparser.h
HEADERS += src/gui/abstract_translated_ui.h
HEADERS += src/gui/forms/askpopupui.h
HEADERS += src/gui/forms/endinstallationui.h
HEADERS += src/gui/forms/installationui.h
HEADERS += src/gui/forms/windowui.h
HEADERS += src/gui/wizard/installwizard.h
HEADERS += src/gui/wizard/pages/folders.h
HEADERS += src/gui/wizard/pages/launch.h
HEADERS += src/gui/wizard/pages/license.h
HEADERS += src/gui/wizard/pages/proxy.h
HEADERS += src/gui/wizard/pages/shortcuts.h
HEADERS += src/gui/wizard/pages/type.h
HEADERS += src/gui/wizard/pages/welcome.h
HEADERS += src/gui/manager/uimanager.h
HEADERS += src/manager/installmanager.h
HEADERS += src/settings/resourcesettings.h
HEADERS += src/utils.h

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

macx {
QMAKE_PRE_LINK += rm -f .DS_Store bin/*.log;
QMAKE_POST_LINK += ../tools/macosx/dmg/build.sh \"$$TARGET\" "background.png";
dmgclean.commands = rm -f $$DESTDIR/$$TARGET\.dmg
distclean.depends += dmgclean
QMAKE_EXTRA_TARGETS += distclean dmgclean
}

#win32 {
#CONFIG(release, debug|release) {
#QMAKE_POST_LINK += ../tools/windows/upx/upx.exe -9 \"$$DESTDIR/"$$TARGET".exe\"
#}
#}

