QT += core

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

TARGET = loader
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
LIBS += -L../_io/bin -lio
INCLUDEPATH += ../_settings/src
LIBS += -L../_settings/bin -lsettings
INCLUDEPATH += ../_logger/src
LIBS += -L../_logger/bin -llogger

SOURCES += src/main.cpp
SOURCES += src/loadermanager.cpp
SOURCES += src/utils.cpp

HEADERS += src/loadermanager.h
HEADERS += src/utils.h

# RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg

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




