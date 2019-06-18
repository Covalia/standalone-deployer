#ifndef IO_OSRESOURCESIMPL_H
#define IO_OSRESOURCESIMPL_H

#include "factories/osresources/osresources.h"
#include "io/config.h"
#include "log/logger.h"
#include "io/fileutils.h"
#include "factories/apppath/apppath.h"

class OsResourcesImpl {
    public:

        OsResourcesImpl(const AppPath * const _appPath);
        virtual ~OsResourcesImpl();

        virtual bool extractResources() = 0;

    protected:
        const AppPath * const m_appPath;
};

#endif
