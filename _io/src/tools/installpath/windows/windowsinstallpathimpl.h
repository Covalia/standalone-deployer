#ifndef IO__WINDOWSINSTALLPATHIMPL_H
#define IO__WINDOWSINSTALLPATHIMPL_H

#include "tools/installpath/installpathimpl.h"

class WindowsInstallPathImpl : public InstallPathImpl {
    public:
        WindowsInstallPathImpl();
        virtual ~WindowsInstallPathImpl();
        virtual QString getInstallationRootPath(FileSystemConfig::AppComponent _app);
};

#endif
