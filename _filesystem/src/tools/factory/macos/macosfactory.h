#ifndef FILESYSTEM__MACOSFACTORY_H
#define FILESYSTEM__MACOSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/path/pathimpl.h"

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
        virtual PathImpl * makePath();

    private:
        MacosFactory();
};

#endif
