#ifndef INSTALLER_MACOSRESOURCESIMPL_H
#define INSTALLER_MACOSRESOURCESIMPL_H

#include "installerfactories/osresources/osresourcesimpl.h"
#include "factories/apppath/apppath.h"

class MacosResourcesImpl : public OsResourcesImpl {
    public:
        MacosResourcesImpl(const QString &_installPath);
        virtual bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        static bool writeFileToTempFile(const QString &_resourceFile, const QString &_toPath);

        AppPath m_appPath;
};

#endif
