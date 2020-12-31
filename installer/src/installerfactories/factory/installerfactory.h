#ifndef INSTALLER_FACTORY_H
#define INSTALLER_FACTORY_H

#include <mutex>
#include <QDir>
#include "io/config.h"

class ShortcutImpl;
class OsResourcesImpl;

class InstallerFactory {
    public:
        static InstallerFactory * getFactory();

        // services offered by this factory

        // overridable specific os resources
        virtual OsResourcesImpl * makeOsResources(const QString &_installPath) = 0;

        // manage shortcuts
        virtual ShortcutImpl * makeShortcut() = 0;

    protected:
        static std::mutex sm_mutex;
};

#endif
