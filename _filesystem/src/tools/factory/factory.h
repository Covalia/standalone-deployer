#ifndef FILESYSTEM__FACTORY_H
#define FILESYSTEM__FACTORY_H

#include <mutex>

class PathImpl;

class Factory {
    public:
        // le getter sur la factory
        static Factory * getFactory();

        // les services proposés par la factory

        // gestion des working directories
        virtual PathImpl * makePath() = 0;

    protected:
        static std::mutex sm_mutex;
};

#endif
