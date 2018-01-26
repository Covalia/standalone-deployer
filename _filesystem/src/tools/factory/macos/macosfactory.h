#ifndef FILESYSTEM__MACOSFACTORY_H
#define FILESYSTEM__MACOSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/path/pathimpl.h"

// TODO voir comment faire un lock + mutex

class MacosFactory : public Factory {
    public:
        static MacosFactory * getFactoryInstance()
        {
            static MacosFactory * factory = 0;

            if (!factory) {
                factory = new MacosFactory;
            }
            return factory;
        }

        virtual ~MacosFactory();
        virtual PathImpl * makePath();

    private:
        MacosFactory();
};

#endif
