#ifndef INSTALLER_MACOSRESOURCES_H
#define INSTALLER_MACOSRESOURCES_H

#include <QString>

#include "factories/apppath/apppath.h"

class MacosResources {
    public:
        MacosResources(const AppPath _appPath);
        ~MacosResources();

        bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        static bool writeFileToTempFile(const QString &_resourceFile, const QString &_toPath);

        const AppPath m_appPath;
};

#endif
