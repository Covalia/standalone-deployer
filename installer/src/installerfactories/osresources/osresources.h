#ifndef INSTALLER_OSRESOURCES_H
#define INSTALLER_OSRESOURCES_H

#include <QString>
#include <QSharedPointer>
#include <QTemporaryFile>
#include "installerfactories/osresources/osresourcesimpl.h"

class OsResourcesImpl;

class OsResources {
    public:
        OsResources(const QString &_installPath);
        virtual ~OsResources();

        bool extractResources();

        static bool extractProjectIniToTempFile(const QString &_toPath);
        static bool extractStyleCssToTempFile(const QString &_toPath);
        static bool extractTitlePngToTempFile(const QString &_toPath);

    private:
        QSharedPointer<OsResourcesImpl> m_osResourcesImpl;
};

#endif
