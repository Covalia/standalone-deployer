INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/src

DEPENDPATH += $$PWD
DEPENDPATH += $$PWD/src

INCLUDEPATH += ../logger/src
LIBS += -L../logger/bin -llogger
INCLUDEPATH += ../settings/src
LIBS += -L../settings/bin -lsettings

FORMS += $$PWD/ui/about.ui
FORMS += $$PWD/ui/askpopup.ui
FORMS += $$PWD/ui/downloadui.ui
FORMS += $$PWD/ui/personalize.ui
FORMS += $$PWD/ui/proxy.ui
FORMS += $$PWD/ui/welcome.ui
FORMS += $$PWD/ui/window.ui

SOURCES += $$PWD/src/gui/about.cpp
SOURCES += $$PWD/src/gui/askpopup.cpp
SOURCES += $$PWD/src/gui/downloadui.cpp
SOURCES += $$PWD/src/gui/personalize.cpp
SOURCES += $$PWD/src/gui/proxy.cpp
SOURCES += $$PWD/src/gui/welcome.cpp
SOURCES += $$PWD/src/gui/window.cpp
SOURCES += $$PWD/src/language/languagemanager.cpp
SOURCES += $$PWD/src/style/stylemanager.cpp
SOURCES += $$PWD/src/uimanager/uimanager.cpp

HEADERS += $$PWD/src/gui/about.h
HEADERS += $$PWD/src/gui/askpopup.h
HEADERS += $$PWD/src/gui/downloadui.h
HEADERS += $$PWD/src/gui/personalize.h
HEADERS += $$PWD/src/gui/proxy.h
HEADERS += $$PWD/src/gui/welcome.h
HEADERS += $$PWD/src/gui/window.h
HEADERS += $$PWD/src/language/languagemanager.h
HEADERS += $$PWD/src/style/stylemanager.h
HEADERS += $$PWD/src/uimanager/uimanager.h

RESOURCES += $$PWD/resources_gui.qrc
