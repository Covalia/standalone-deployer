#ifndef INSTALLER_MACOSFACTORY_H
#define INSTALLER_MACOSFACTORY_H

#include "installerfactories/factory/installerfactory.h"
#include "installerfactories/osresources/osresourcesimpl.h"
#include "installerfactories/shortcut/shortcutimpl.h"

class MacosInstallerFactory : public InstallerFactory {
    public:
        static MacosInstallerFactory * getFactoryInstance()
        {
            static MacosInstallerFactory * factory = nullptr;

            sm_mutex.lock();
            if (!factory) {
                factory = new MacosInstallerFactory;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~MacosInstallerFactory();
        virtual OsResourcesImpl * makeOsResources(const QString &_installPath);
        virtual ShortcutImpl * makeShortcut();

    private:
        MacosInstallerFactory();
};

#endif
