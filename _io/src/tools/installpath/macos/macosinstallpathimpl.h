#ifndef IO__MACOSINSTALLPATHIMPL_H
#define IO__MACOSINSTALLPATHIMPL_H

#include "tools/installpath/installpathimpl.h"

class MacosInstallPathImpl : public InstallPathImpl {
    public:
        MacosInstallPathImpl();
        virtual ~MacosInstallPathImpl();
        virtual QString getInstallationRootPath(FileSystemConfig::AppComponent _app);
};

#endif
