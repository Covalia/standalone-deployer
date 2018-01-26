#ifndef FILESYSTEM__PATHIMPL_H
#define FILESYSTEM__PATHIMPL_H

#include "tools/path/path.h"

#include <QCoreApplication>

class PathImpl {
    public:
        // Tout le code en commun vient ici
        PathImpl();
        virtual ~PathImpl();
        virtual QString getInstallationRootPath() = 0;
};

#endif
