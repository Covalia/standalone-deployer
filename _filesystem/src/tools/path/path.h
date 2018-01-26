#ifndef FILESYSTEM__PATH_H
#define FILESYSTEM__PATH_H

#include <QString>
#include "tools/path/pathimpl.h"

class PathImpl;

class Path {
    public:
        Path();
        virtual ~Path();
        // chaque application doit implémenter ceci pour trouver la
        // racine du projet à partir
        QString getInstallationRoot();

    private:
        PathImpl * m_pathImpl;
};

#endif
