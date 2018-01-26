#ifndef FILESYSTEM__WINDOWSFACTORY_H
#define FILESYSTEM__WINDOWSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/path/pathimpl.h"

class WindowsFactory : public Factory {
    public:
        static WindowsFactory * getFactoryInstance()
        {
            static WindowsFactory * factory = 0;

            sm_mutex.lock();
            if (!factory) {
                factory = new MacosFactory;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~WindowsFactory();
        virtual PathImpl * makePath();

    private:
        WindowsFactory();
};

#endif
