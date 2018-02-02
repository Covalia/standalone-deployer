#ifndef IO__WINDOWSFACTORY_H
#define IO__WINDOWSFACTORY_H

#include "tools/factory/factory.h"
#include "tools/installpath/installpathimpl.h"
#include "tools/shortcut/shortcutimpl.h"

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
        virtual InstallPathImpl * makeInstallPath(FileSystemConfig::AppComponent _app);
        virtual ShortcutImpl * makeShortcut();

    private:
        WindowsFactory();
};

#endif
