#ifndef IO__FACTORY_H
#define IO__FACTORY_H

#include <mutex>
#include <QDir>
#include "io/config.h"

class AppPathImpl;
class ShortcutImpl;

class Factory {
    public:
        // le getter sur la factory
        static Factory * getFactory();

        // les services proposés par la factory

        // gestion des working directories
        virtual AppPathImpl * makeAppPath(IOConfig::AppComponent _app) = 0;
        // gestion des raccourcis
        virtual ShortcutImpl * makeShortcut() = 0;

    protected:
        static std::mutex sm_mutex;
};

#endif
