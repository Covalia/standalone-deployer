#ifndef IO_FACTORY_H
#define IO_FACTORY_H

#include <mutex>
#include <QDir>
#include "io/config.h"

class AppPathImpl;

class Factory {
    public:
        static Factory * getFactory();

        // services offered by this factory

        // manage working directories
        virtual AppPathImpl * makeAppPath(IOConfig::AppComponent _app) = 0;

    protected:
        static std::mutex sm_mutex;
};

#endif
