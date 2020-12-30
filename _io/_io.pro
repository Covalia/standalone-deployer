include("../standalone-deployer-version.pri")
QT += core
QT += concurrent

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += staticlib

CONFIG(release, debug|release) {
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = io
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
INCLUDEPATH += headers/libarchive

DEPENDPATH += .
DEPENDPATH += src

INCLUDEPATH += ../_logger/src

SOURCES += src/io/config.cpp
SOURCES += src/io/fileutils.cpp
SOURCES += src/io/info.cpp
SOURCES += src/io/unzip/zipextractor.cpp
SOURCES += src/factories/factory/factory.cpp
SOURCES += src/factories/apppath/apppath.cpp
SOURCES += src/factories/apppath/apppathimpl.cpp

HEADERS += src/build_hash.h
HEADERS += src/io/config.h
HEADERS += src/io/fileutils.h
HEADERS += src/io/info.h
HEADERS += src/io/unzip/qarchive/qarchive.h
HEADERS += src/io/unzip/zipextractor.h
HEADERS += src/factories/factory/factory.h
HEADERS += src/factories/apppath/apppath.h
HEADERS += src/factories/apppath/apppathimpl.h

win32 {
	HEADERS += src/factories/factory/windows/windowsfactory.h
	HEADERS += src/factories/apppath/windows/windowsapppathimpl.h

	SOURCES += src/factories/factory/windows/windowsfactory.cpp
	SOURCES += src/factories/apppath/windows/windowsapppathimpl.cpp
}

macx {
	HEADERS += src/factories/factory/macos/macosfactory.h
	HEADERS += src/factories/apppath/macos/macosapppathimpl.h

	SOURCES += src/factories/factory/macos/macosfactory.cpp
	SOURCES += src/factories/apppath/macos/macosapppathimpl.cpp
}

# get hash from git
BUILD_HASH = $$system("git rev-parse --short HEAD")

message(">>> Build from revision: $$BUILD_HASH")

build_hash.target = src/build_hash.h
build_hash.depends = FORCE
macx {
	build_hash.commands = echo \"$${LITERAL_HASH}define STANDALONE_DEPLOYER_BUILD_HASH \\\"$$BUILD_HASH\\\"\" > src/build_hash.h
}
win32 {
	build_hash.commands = echo $${LITERAL_HASH}define STANDALONE_DEPLOYER_BUILD_HASH \"$$BUILD_HASH\" > src/build_hash.h
}
PRE_TARGETDEPS += src/build_hash.h
QMAKE_EXTRA_TARGETS += build_hash

