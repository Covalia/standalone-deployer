#ifndef IO_WINDOWSFACTORY_H
#define IO_WINDOWSFACTORY_H

#include "factories/factory/factory.h"
#include "factories/apppath/apppathimpl.h"

class WindowsFactory : public Factory {
    public:
        static WindowsFactory * getFactoryInstance()
        {
            static WindowsFactory * factory = nullptr;

            sm_mutex.lock();
            if (!factory) {
                factory = new WindowsFactory;
            }
            sm_mutex.unlock();

            return factory;
        }

        virtual ~WindowsFactory();
        virtual AppPathImpl * makeAppPath(IOConfig::AppComponent _app);

    private:
        WindowsFactory();
};

#endif
