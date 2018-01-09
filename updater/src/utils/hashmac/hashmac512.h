#ifndef UPDATER__HASHMAC512_H
#define UPDATER__HASHMAC512_H

#include <QString>

class HashMac512 {
    public:
        static QString hashFromFile(const QString &_filePath, const QString &_secret);
};

#endif
