#ifndef IO_MACOSRESOURCESIMPL_H
#define IO_MACOSRESOURCESIMPL_H

#include "factories/osresources/osresourcesimpl.h"

class AppPath;

class MacosResourcesImpl : public OsResourcesImpl {
    public:
        MacosResourcesImpl(const AppPath * const _appPath);
        virtual bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        static bool writeFileToTempFile(const QString &_resourceFile, const QString &_toPath);
};

#endif
