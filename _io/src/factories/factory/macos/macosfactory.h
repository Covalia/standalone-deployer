#ifndef IO_MACOSFACTORY_H
#define IO_MACOSFACTORY_H

#include "factories/factory/factory.h"
#include "factories/apppath/apppathimpl.h"
#include "factories/shortcut/shortcutimpl.h"

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
        virtual AppPathImpl * makeAppPath(IOConfig::AppComponent _app);
        virtual ShortcutImpl * makeShortcut();

    private:
        MacosFactory();
};

#endif
