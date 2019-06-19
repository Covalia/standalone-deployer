#ifndef INSTALLER_WINDOWSFACTORY_H
#define INSTALLER_WINDOWSFACTORY_H

#include "installerfactories/factory/installerfactory.h"
#include "installerfactories/osresources/osresourcesimpl.h"

class WindowsInstallerFactory : public InstallerFactory {
    public:
        static WindowsInstallerFactory * getFactoryInstance()
        {
            static WindowsInstallerFactory * factory = nullptr;

            sm_mutex.lock();
            if (!factory) {
                factory = new WindowsInstallerFactory;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~WindowsInstallerFactory();
        virtual OsResourcesImpl * makeOsResources(const QString &_installPath);

    private:
        WindowsInstallerFactory();
};

#endif
