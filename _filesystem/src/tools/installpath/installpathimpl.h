#ifndef FILESYSTEM__PATHIMPL_H
#define FILESYSTEM__PATHIMPL_H

#include "tools/installpath/installpath.h"
#include "fs/config.h"

#include <QCoreApplication>
#include <QDir>

class InstallPathImpl {
    public:
        // Tout le code en commun vient ici
        InstallPathImpl();
        virtual ~InstallPathImpl();
        virtual QString getInstallationRootPath(FileSystemConfig::AppComponent _app) = 0;
    protected:
        bool cdUp(QDir &_dir, int _numUp);
};

#endif
