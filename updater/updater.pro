QT += core
QT += gui
QT += widgets
QT += network
QT += concurrent

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

TARGET = updater
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
INCLUDEPATH += headers/libarchive

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../_logger/src
LIBS += -L../_logger/bin -llogger
INCLUDEPATH += ../_settings/src
LIBS += -L../_settings/bin -lsettings
INCLUDEPATH += ../_filesystem/src
LIBS += -L../_filesystem/bin -lfilesystem

macx {
LIBS += -L./libs/libarchive/macosx -larchive
}
win32 {
# attention, l'ordre est important.
LIBS += -L./libs/libarchive/win32 -larchive -lbz2 -lxml2 -lz -llzma -llz4 -lnettle -llzo
}

FORMS += ui/askpopup.ui
FORMS += ui/authenticationdialog.ui
FORMS += ui/mainwindow.ui
FORMS += ui/splashscreen.ui

SOURCES += src/main.cpp
SOURCES += src/gui/askpopupui.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/gui/splashscreen.cpp
SOURCES += src/gui/style/stylemanager.cpp
SOURCES += src/network/downloadmanager.cpp
SOURCES += src/utils/hashmac/hashmac512.cpp
SOURCES += src/utils/unzip/zipextractor.cpp
SOURCES += src/updater/appupdater.cpp
SOURCES += src/updater/config.cpp
SOURCES += src/xml/data/application.cpp
SOURCES += src/xml/data/download.cpp
SOURCES += src/xml/data/javaupdate.cpp
SOURCES += src/xml/deploymentxml.cpp

HEADERS += src/gui/askpopupui.h
HEADERS += src/gui/mainwindow.h
HEADERS += src/gui/splashscreen.h
HEADERS += src/gui/style/stylemanager.h
HEADERS += src/network/downloadmanager.h
HEADERS += src/utils/hashmac/hashmac512.h
HEADERS += src/utils/qarchive/qarchive.h
HEADERS += src/utils/unzip/zipextractor.h
HEADERS += src/updater/appupdater.h
HEADERS += src/updater/config.h
HEADERS += src/xml/data/application.h
HEADERS += src/xml/data/download.h
HEADERS += src/xml/data/javaupdate.h
HEADERS += src/xml/deploymentxml.h

RESOURCES += fixed_resources.qrc

defined(OVERRIDABLE_UPDATER_RESOURCES, var) {
    RESOURCES += $$OVERRIDABLE_UPDATER_RESOURCES
    message("Using installer resources from file: $$OVERRIDABLE_UPDATER_RESOURCES")
}
else {
    RESOURCES += overridable_resources.qrc
}



DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/fr_FR.ts
TRANSLATIONS += resources/lang/en_US.ts

macx {
QMAKE_POST_LINK += ../tools/macosx/dmg/build.sh \"$$TARGET\" "background-no-run.png"
dmgclean.commands = rm -f $$DESTDIR/$$TARGET\.dmg
distclean.depends += dmgclean
QMAKE_EXTRA_TARGETS += distclean dmgclean
}

win32 {
CONFIG(release, debug|release) {
QMAKE_POST_LINK += ../tools/windows/upx/upx.exe -9 \"$$DESTDIR/"$$TARGET".exe\"
}
}

