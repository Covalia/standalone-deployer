#ifndef IO__FACTORY_H
#define IO__FACTORY_H

#include <mutex>

class InstallPathImpl;
class ShortcutImpl;

class Factory {
    public:
        // le getter sur la factory
        static Factory * getFactory();

        // les services proposés par la factory

        // gestion des working directories
        virtual InstallPathImpl * makeInstallPath() = 0;
        // gestion des raccourcis
        virtual ShortcutImpl * makeShortcut() = 0;

    protected:
        static std::mutex sm_mutex;
};

#endif
