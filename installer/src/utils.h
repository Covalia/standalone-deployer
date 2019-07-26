#ifndef INSTALLER_UTILS_H
#define INSTALLER_UTILS_H

#include "factories/apppath/apppath.h"

#include <QString>

class Utils {
    public:
        static AppPath getAppPath();
        static QString getInstallerlLogPath();
};

#endif
