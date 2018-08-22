#ifndef UPDATER__HASH_KEY_H
#define UPDATER__HASH_KEY_H

#include <QString>

class KeyManager {
    public:
        static QString readHashKey();
        static QString readPasswordEncryptionKey();

    private:
        KeyManager();
        virtual ~KeyManager();

        static QString getNamedKey(const QString &_keyFileName);
};

#endif
