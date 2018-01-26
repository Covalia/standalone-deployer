#ifndef FILESYSTEM__WINDOWSPATHIMPL_H
#define FILESYSTEM__WINDOWSPATHIMPL_H

#include "tools/path/pathimpl.h"

class WindowsPathImpl : public PathImpl {
    public:
        WindowsPathImpl();
        virtual ~WindowsPathImpl();
        virtual QString getInstallationRootPath();
};

#endif
