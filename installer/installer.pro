QT += core
QT += gui
QT += widgets

CONFIG += warn_on
CONFIG += debug_and_release
CONFIG += app_bundle
CONFIG += resources_big

QMAKE_TARGET_BUNDLE_PREFIX = com.covalia.standalonedeployer

CONFIG(debug, release|debug) {
	CONFIG += console
}

CONFIG(release, debug|release) {
	CONFIG -= console
	DEFINES += QT_NO_DEBUG_OUTPUT
}

TARGET = installer
TEMPLATE = app

# icone macos
defined(INSTALLER_MACOS_ICON, var) {
    ICON = "$$INSTALLER_MACOS_ICON"
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

FORMS += ui/askpopup.ui
FORMS += ui/endinstallation.ui
FORMS += ui/installation.ui

SOURCES += src/main.cpp
SOURCES += src/commandline/commandlineparser.cpp
SOURCES += src/installerfactories/factory/installerfactory.cpp
SOURCES += src/installerfactories/osresources/osresources.cpp
SOURCES += src/installerfactories/osresources/osresourcesimpl.cpp
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
SOURCES += src/gui/wizard/pages/thirdparty.cpp
SOURCES += src/gui/wizard/pages/type.cpp
SOURCES += src/gui/wizard/pages/welcome.cpp
SOURCES += src/gui/manager/uimanager.cpp
SOURCES += src/manager/installmanager.cpp
SOURCES += src/settings/resourcesettings.cpp
SOURCES += src/utils.cpp

HEADERS += src/commandline/commandlineparser.h
HEADERS += src/installerfactories/factory/installerfactory.h
HEADERS += src/installerfactories/osresources/osresources.h
HEADERS += src/installerfactories/osresources/osresourcesimpl.h
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
HEADERS += src/gui/wizard/pages/thirdparty.h
HEADERS += src/gui/wizard/pages/type.h
HEADERS += src/gui/wizard/pages/welcome.h
HEADERS += src/gui/manager/uimanager.h
HEADERS += src/manager/installmanager.h
HEADERS += src/settings/resourcesettings.h
HEADERS += src/utils.h

RESOURCES += fixed_resources.qrc

win32 {
	RESOURCES += windows_resources.qrc

	HEADERS += src/installerfactories/factory/windows/windowsinstallerfactory.h
	HEADERS += src/installerfactories/osresources/windows/windowsresourcesimpl.h

	SOURCES += src/installerfactories/factory/windows/windowsinstallerfactory.cpp
	SOURCES += src/installerfactories/osresources/windows/windowsresourcesimpl.cpp
}

macx {
	RESOURCES += macos_resources.qrc

	HEADERS += src/installerfactories/factory/macos/macosinstallerfactory.h
	HEADERS += src/installerfactories/osresources/macos/macosresourcesimpl.h

	SOURCES += src/installerfactories/factory/macos/macosinstallerfactory.cpp
	SOURCES += src/installerfactories/osresources/macos/macosresourcesimpl.cpp
}

DISTFILES += ../uncrustify.cfg
TRANSLATIONS += resources/lang/app/fr_FR.ts
TRANSLATIONS += resources/lang/app/en_US.ts

RC_FILE = windows_resources.rc

macx {
	CONFIG(release, debug|release) {
		!defined(SIGNATURE_IDENTITY, var) {
			error(SIGNATURE_IDENTITY must be specified in order to sign app and dmg files.)
		}
	}

	QMAKE_PRE_LINK += rm -f .DS_Store bin/*.log;

	QMAKE_POST_LINK += cp resources/project.ini \"$$DESTDIR/$$TARGET\".app/Contents/Resources/
	QMAKE_POST_LINK += $$escape_expand(\n\t)cp resources/style.css \"$$DESTDIR/$$TARGET\".app/Contents/Resources/
	QMAKE_POST_LINK += $$escape_expand(\n\t)cp -r resources/overridable/images/ \"$$DESTDIR/$$TARGET\".app/Contents/Resources/

	CONFIG(release, debug|release) {
		defined(SIGNATURE_IDENTITY, var) {
			equals(SIGN_INSTALLER, "false") {
				message("Do not sign the installer.")
			} else {
				QMAKE_POST_LINK += $$escape_expand(\n\t)codesign --force -i \"$$QMAKE_TARGET_BUNDLE_PREFIX\".\"$$TARGET\" --deep --sign \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/$$TARGET\".app
			}
		}
	}

	QMAKE_POST_LINK += $$escape_expand(\n\t)$$(HOME)/.virtualenvs/standalone-deployer/bin/dmgbuild -s ../tools/macos/dmg/dmgbuild-settings.py -D background="../tools/macos/dmg/background.png" \"$$TARGET\" \"$$DESTDIR/$$TARGET\".dmg

	CONFIG(release, debug|release) {
		defined(SIGNATURE_IDENTITY, var) {
			equals(SIGN_INSTALLER, "false") {
				message("Do not sign the installer.")
			} else {
				QMAKE_POST_LINK += $$escape_expand(\n\t)codesign --force -i \"$$QMAKE_TARGET_BUNDLE_PREFIX\".\"$$TARGET\".dmg --deep --sign \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/$$TARGET\".dmg
			}
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

		defined(SIGNATURE_IDENTITY, var) {
			equals(SIGN_INSTALLER, "false") {
				message("Do not sign the installer.")
			} else {
				# signtool.exe must be in the PATH
				QMAKE_POST_LINK += $$escape_expand(\n\t)signtool.exe sign /t http://timestamp.digicert.com /n \"$$SIGNATURE_IDENTITY\" \"$$DESTDIR/"$$TARGET".exe\"
			}
		}
		else {
			error(SIGNATURE_IDENTITY must be specified in order to sign exe files.)
		}
	}
}

