#ifndef FILESYSTEM__MACOSPATHIMPL_H
#define FILESYSTEM__MACOSPATHIMPL_H

#include "tools/path/pathimpl.h"

class MacosPathImpl : public PathImpl {
    public:
        MacosPathImpl();
        virtual ~MacosPathImpl();
        virtual QString getInstallationRoot();
};

#endif
