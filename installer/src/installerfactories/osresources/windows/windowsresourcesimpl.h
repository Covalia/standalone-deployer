#ifndef INSTALLER_WINDOWSRESOURCESIMPL_H
#define INSTALLER_WINDOWSRESOURCESIMPL_H

#include "installerfactories/osresources/osresourcesimpl.h"
#include <windows.h>

class WindowsResourcesImpl : public OsResourcesImpl {
    public:
        WindowsResourcesImpl(const QString &_installPath);
        virtual bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        static BOOL EnumNamesFunc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam);
        static bool writeResourceIdToFile(LPCWSTR _resId, const QString &_toPath);

        QString m_installPath;
};

#endif
