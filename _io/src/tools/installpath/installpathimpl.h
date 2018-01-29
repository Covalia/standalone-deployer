#ifndef IO__INSTALLPATHIMPL_H
#define IO__INSTALLPATHIMPL_H

#include "tools/installpath/installpath.h"
#include "io/config.h"

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
