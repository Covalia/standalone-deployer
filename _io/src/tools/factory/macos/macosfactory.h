#ifndef IO__MACOSFACTORY_H
#define IO__MACOSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/installpath/installpathimpl.h"

class MacosFactory : public Factory {
    public:
        static MacosFactory * getFactoryInstance()
        {
            static MacosFactory * factory = 0;

            sm_mutex.lock();
            if (!factory) {
                factory = new MacosFactory;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~MacosFactory();
        virtual InstallPathImpl * makeInstallPath();

    private:
        MacosFactory();
};

#endif
