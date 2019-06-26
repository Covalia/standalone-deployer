QT += core

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += app_bundle

QMAKE_TARGET_BUNDLE_PREFIX = com.covalia.standalonedeployer

CONFIG(debug, release|debug) {
	CONFIG += console
}

CONFIG(release, debug|release) {
	CONFIG -= console
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = loader
TEMPLATE = app

# icone macos
defined(LOADER_MACOS_ICON, var) {
    ICON = "$$LOADER_MACOS_ICON"
}
else {
    ICON = "$$TARGET".icns
}

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
	LIBS += -L./libs/libarchive/macos -larchive
}
win32 {
	# attention, l'ordre est important.
	LIBS += -L./libs/libarchive/win32 -larchive -lbz2 -lxml2 -lz -llzma -llz4 -lnettle -llzo
}

SOURCES += src/main.cpp
SOURCES += src/loadermanager.cpp
SOURCES += src/utils.cpp

HEADERS += src/loadermanager.h
HEADERS += src/utils.h

# RESOURCES += resources.qrc

DISTFILES += ../uncrustify.cfg

macx {
	CONFIG(release, debug|release) {
		!defined(SIGNATURE_IDENTITY, var) {
			error(SIGNATURE_IDENTITY must be specified in order to sign app and dmg files.)
		}
	}

	QMAKE_PRE_LINK += rm -f .DS_Store bin/*.log;

	CONFIG(release, debug|release) {
		defined(SIGNATURE_IDENTITY, var) {
			QMAKE_POST_LINK += codesign --force -i \"$$QMAKE_TARGET_BUNDLE_PREFIX\".\"$$TARGET\" --deep --sign \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/$$TARGET\".app
		}
	}

	QMAKE_POST_LINK += $$escape_expand(\n\t) $$(HOME)/.virtualenvs/standalone-deployer/bin/dmgbuild -s ../tools/macos/dmg/dmgbuild-settings.py -D background="../tools/macos/dmg/background-no-run.png" \"$$TARGET\" \"$$DESTDIR/$$TARGET\".dmg

	CONFIG(release, debug|release) {
		defined(SIGNATURE_IDENTITY, var) {
			QMAKE_POST_LINK += $$escape_expand(\n\t) codesign --force -i \"$$QMAKE_TARGET_BUNDLE_PREFIX\".\"$$TARGET\".dmg --deep --sign \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/$$TARGET\".dmg
		}
	}

	dmgclean.commands = rm -f \"$$DESTDIR/$$TARGET\".dmg
	distclean.depends += dmgclean

	QMAKE_EXTRA_TARGETS += distclean dmgclean
}

win32 {
	CONFIG(release, debug|release) {
		# mt.exe must be in the PATH
		QMAKE_POST_LINK += $$escape_expand(\n\t)mt.exe -nologo -manifest \"$$TARGET\".exe.manifest -outputresource:\"$$DESTDIR/"$$TARGET".exe;$${LITERAL_HASH}1\"

		QMAKE_POST_LINK += $$escape_expand(\n\t)../tools/windows/upx/upx.exe -9 \"$$DESTDIR/"$$TARGET".exe\"
		defined(SIGNATURE_IDENTITY, var) {
			# signtool.exe must be in the PATH
			QMAKE_POST_LINK += $$escape_expand(\n\t) signtool.exe sign /t http://timestamp.digicert.com /n \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/"$$TARGET".exe\"
		}
		else {
			error(SIGNATURE_IDENTITY must be specified in order to sign exe files.)
		}
	}
}

