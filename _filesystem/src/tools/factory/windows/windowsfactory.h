#ifndef FILESYSTEM__WINDOWSFACTORY_H
#define FILESYSTEM__WINDOWSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/path/pathimpl.h"

// TODO voir comment faire un lock + mutex

class WindowsFactory : public Factory {
    public:
        static WindowsFactory * getFactoryInstance()
        {
            static WindowsFactory * factory = 0;

            if (!factory) {
                factory = new WindowsFactory;
            }
            return factory;
        }

        virtual ~WindowsFactory();
        virtual PathImpl * makePath();

    private:
        WindowsFactory();
};

#endif
