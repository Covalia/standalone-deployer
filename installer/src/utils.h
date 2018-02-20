#ifndef LOADER__UTILS_H
#define LOADER__UTILS_H

#include "factories/apppath/apppath.h"

#include <QString>

class Utils {
    public:
        static AppPath getAppPath();
        static QString getInstallerlLogPath();
};

#endif
