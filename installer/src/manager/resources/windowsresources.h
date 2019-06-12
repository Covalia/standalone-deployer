#ifndef INSTALLER_WINDOWSRESOURCES_H
#define INSTALLER_WINDOWSRESOURCES_H

#include <windows.h>

#include <QString>
#include <QByteArray>
#include <QFile>
#include <QDir>

#include "log/logger.h"
#include "io/fileutils.h"

class WindowsResources {
    public:
        WindowsResources(const QString _installPpath);
        ~WindowsResources();

        bool extractResources();
        bool extractProjectIniToTempFile(const QString &_path);


    private:
        static BOOL EnumNamesFunc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam);
        static bool writeResourceIdToFile(LPCWSTR _resId, const QString &_path);

        const QString m_installPath;
};

#endif
