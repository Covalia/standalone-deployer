#ifndef IO__MACOSINSTALLPATHIMPL_H
#define IO__MACOSINSTALLPATHIMPL_H

#include "tools/installpath/installpathimpl.h"

class MacosInstallPathImpl : public InstallPathImpl {
    public:
        MacosInstallPathImpl(FileSystemConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QString getLoaderResourcesPath();
        virtual QString getUpdaterResourcesPath();
        virtual QString getLoaderFilePath();
        virtual QString getUpdaterFilePath(QString updaterVersion);
};

#endif
