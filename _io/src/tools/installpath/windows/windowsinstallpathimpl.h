#ifndef IO__WINDOWSINSTALLPATHIMPL_H
#define IO__WINDOWSINSTALLPATHIMPL_H

#include "tools/installpath/installpathimpl.h"

class WindowsInstallPathImpl : public InstallPathImpl {
    public:
        WindowsInstallPathImpl(FileSystemConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QString getLoaderResourcesPath();
        virtual QString getUpdaterResourcesPath();
        virtual QString getLoaderFilePath();
        virtual QString getUpdaterFilePath(QString updaterVersion);
};

#endif
