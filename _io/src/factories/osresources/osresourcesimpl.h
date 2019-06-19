#ifndef IO_OSRESOURCESIMPL_H
#define IO_OSRESOURCESIMPL_H

#include "factories/osresources/osresources.h"
#include "io/config.h"
#include "log/logger.h"
#include "io/fileutils.h"

class OsResourcesImpl {
    public:

        OsResourcesImpl(const QString &_installPath);
        virtual ~OsResourcesImpl();

        virtual bool extractResources() = 0;
};

#endif
