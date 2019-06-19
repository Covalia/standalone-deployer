#ifndef IO_WINDOWSFACTORY_H
#define IO_WINDOWSFACTORY_H

#include "factories/factory/factory.h"
#include "factories/apppath/apppathimpl.h"
#include "factories/osresources/osresourcesimpl.h"
#include "factories/shortcut/shortcutimpl.h"

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
        virtual ShortcutImpl * makeShortcut();
        virtual OsResourcesImpl * makeOsResources(const QString &_installPath);

    private:
        WindowsFactory();
};

#endif
