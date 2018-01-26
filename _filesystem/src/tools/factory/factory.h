#ifndef FILESYSTEM__FACTORY_H
#define FILESYSTEM__FACTORY_H

class PathImpl;

class Factory {
    public:
        // le getter sur la factory
        static Factory * getFactory();

        // les services proposés par la factory

        // TODO gestion des working directories
        virtual PathImpl * makePath() = 0;
};

#endif
