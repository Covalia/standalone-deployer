#ifndef INSTALLER_OSRESOURCES_H
#define INSTALLER_OSRESOURCES_H

#include <QTemporaryFile>

#ifdef Q_OS_WIN
#include "manager/resources/windowsresources.h"
#endif
#ifdef Q_OS_MACOS
#include "manager/resources/macosresources.h"
#endif

class OsResources {
    public:
        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);
};

#endif
