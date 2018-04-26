#ifndef UPDATER__HASH_KEY_H
#define UPDATER__HASH_KEY_H

#include <QString>

class HashKey {
    public:
        static QString readHashKey();

    private:
        HashKey();
        ~HashKey();
};

#endif
