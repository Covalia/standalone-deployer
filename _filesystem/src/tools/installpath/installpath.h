#ifndef FILESYSTEM__PATH_H
#define FILESYSTEM__PATH_H

#include <QString>
#include "tools/installpath/installpathimpl.h"
#include "fs/config.h"

class InstallPathImpl;

class InstallPath {
    public:
        InstallPath();
        virtual ~InstallPath();
        // chaque application doit implémenter ceci pour trouver la
        // racine du projet à partir
        QString getInstallationRootPath(FileSystemConfig::AppComponent _app);

    private:
        InstallPathImpl * m_pathInstallImpl;
};

#endif
