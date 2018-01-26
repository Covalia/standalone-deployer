#ifndef FILESYSTEM__MACOSPATHIMPL_H
#define FILESYSTEM__MACOSPATHIMPL_H

#include "tools/installpath/installpathimpl.h"

class MacosInstallPathImpl : public InstallPathImpl {
    public:
        MacosInstallPathImpl();
        virtual ~MacosInstallPathImpl();
        virtual QString getInstallationRootPath(FileSystemConfig::AppComponent _app);
};

#endif
