include("../standalone-deployer-version.pri")
QT += core
QT += gui
QT += widgets
QT += network

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

TARGET = updater
TEMPLATE = app

# macos icon
defined(UPDATER_MACOS_ICON, var) {
    ICON = "$$UPDATER_MACOS_ICON"
}
else {
    ICON = "$$TARGET".icns
}

# windows icon
defined(UPDATER_WINDOWS_ICON, var) {
	RC_ICONS = "$$UPDATER_WINDOWS_ICON"
}
else {
	RC_ICONS = "$$TARGET".ico
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

# warning, order is important.
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
	# warning, order is important.
	LIBS += -L./libs/libarchive/win32 -larchive -lbz2 -lxml2 -lz -llzma -llz4 -lnettle -llzo
}

FORMS += ui/askpopup.ui
FORMS += ui/authenticationdialog.ui
FORMS += ui/mainwindow.ui

SOURCES += src/main.cpp
SOURCES += src/gui/askpopupui.cpp
SOURCES += src/gui/authenticationdialogui.cpp
SOURCES += src/gui/mainwindow.cpp
SOURCES += src/gui/style/stylemanager.cpp
SOURCES += src/network/downloadmanager.cpp
SOURCES += src/updater/appupdater.cpp
SOURCES += src/updater/config.cpp
SOURCES += src/utils.cpp
SOURCES += src/utils/hashmac/hashmac512.cpp
SOURCES += src/settings/commandlinesingleton.cpp
SOURCES += src/xml/data/application.cpp
SOURCES += src/xml/data/download.cpp
SOURCES += src/xml/data/javaupdate.cpp
SOURCES += src/xml/deploymentxml.cpp

HEADERS += src/gui/askpopupui.h
HEADERS += src/gui/authenticationdialogui.h
HEADERS += src/gui/mainwindow.h
HEADERS += src/gui/style/stylemanager.h
HEADERS += src/network/downloadmanager.h
HEADERS += src/network/downloadmanagererror.h
HEADERS += src/updater/appupdater.h
HEADERS += src/updater/config.h
HEADERS += src/utils.h
HEADERS += src/utils/hashmac/hashmac512.h
HEADERS += src/settings/commandlinesingleton.h
HEADERS += src/xml/data/application.h
HEADERS += src/xml/data/download.h
HEADERS += src/xml/data/javaupdate.h
HEADERS += src/xml/deploymentxml.h

RESOURCES += fixed_resources.qrc

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/app/fr_FR.ts
TRANSLATIONS += resources/lang/app/en_US.ts

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

	QMAKE_POST_LINK += $$escape_expand(\n\t)$$(HOME)/.virtualenvs/standalone-deployer/bin/dmgbuild -s ../tools/macos/dmg/dmgbuild-settings.py -D background="../tools/macos/dmg/background-no-run.png" \"$$TARGET\" \"$$DESTDIR/$$TARGET\".dmg

	CONFIG(release, debug|release) {
		defined(SIGNATURE_IDENTITY, var) {
			QMAKE_POST_LINK += $$escape_expand(\n\t)codesign --force -i \"$$QMAKE_TARGET_BUNDLE_PREFIX\".\"$$TARGET\".dmg --deep --sign \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/$$TARGET\".dmg
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
			QMAKE_POST_LINK += $$escape_expand(\n\t)signtool.exe sign /t http://timestamp.digicert.com /n \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/"$$TARGET".exe\"
		}
		else {
			error(SIGNATURE_IDENTITY must be specified in order to sign exe files.)
		}
	}
}

