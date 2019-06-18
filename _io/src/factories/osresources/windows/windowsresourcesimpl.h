#ifndef IO_WINDOWSRESOURCESIMPL_H
#define IO_WINDOWSRESOURCESIMPL_H

#include "factories/osresources/osresourcesimpl.h"
#include <windows.h>

class AppPath;

class WindowsResourcesImpl : public OsResourcesImpl {
    public:
        WindowsResourcesImpl(const AppPath * const _appPath);
        virtual bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        static BOOL EnumNamesFunc(HMODULE hModule, LPCWSTR lpType, LPWSTR lpName, LONG_PTR lParam);
        static bool writeResourceIdToFile(LPCWSTR _resId, const QString &_toPath);
};

#endif
