INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/src

DEPENDPATH += $$PWD
DEPENDPATH += $$PWD/src

INCLUDEPATH += ../logger/src
LIBS += -L../logger/bin -llogger
INCLUDEPATH += ../settings/src
LIBS += -L../settings/bin -lsettings
