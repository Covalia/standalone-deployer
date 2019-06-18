#ifndef INSTALLER_WINDOWSRESOURCES_H
#define INSTALLER_WINDOWSRESOURCES_H

#include <windows.h>
#include <QString>

#include "factories/apppath/apppath.h"

class WindowsResources {
    public:
        WindowsResources(const AppPath _appPath);
        ~WindowsResources();

        bool extractResources();
        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        static BOOL EnumNamesFunc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam);
        static bool writeResourceIdToFile(LPCWSTR _resId, const QString &_toPath);

        const QString m_installPath;
};

#endif
