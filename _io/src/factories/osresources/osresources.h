#ifndef IO_OSRESOURCES_H
#define IO_OSRESOURCES_H

#include <QString>
#include <QSharedPointer>
#include <QTemporaryFile>
#include "factories/osresources/osresourcesimpl.h"

class OsResourcesImpl;
class AppPath;

class OsResources {
    public:
        OsResources(const AppPath * const _appPath);
        virtual ~OsResources();

        bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        QSharedPointer<OsResourcesImpl> m_osResourcesImpl;
};

#endif
