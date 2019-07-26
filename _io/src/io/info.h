#ifndef IO_INFO_H
#define IO_INFO_H

#include <QString>

class Info {
    public:
        static QString getBuildHash();

    private:
        Info();
        ~Info();
};

#endif
