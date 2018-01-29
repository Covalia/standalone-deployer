#ifndef IO__WINDOWSFACTORY_H
#define IO__WINDOWSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/installpath/installpathimpl.h"

class WindowsFactory : public Factory {
    public:
        static WindowsFactory * getFactoryInstance()
        {
            static WindowsFactory * factory = 0;

            sm_mutex.lock();
            if (!factory) {
                factory = new WindowsFactory;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~WindowsFactory();
        virtual InstallPathImpl * makeInstallPath();

    private:
        WindowsFactory();
};

#endif
