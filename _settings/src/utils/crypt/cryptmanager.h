#ifndef SETTINGS__CRYPTMANAGER_H
#define SETTINGS__CRYPTMANAGER_H

#include <QString>
#include <QByteArray>

class CryptManager
{
    public:
        static QString encrypt(const QString _hexKey, const QString _plaintextMessage);
        static QString decrypt(const QString _hexKey, const QString _encryptedHexMessage);

    private:
        CryptManager();
        virtual ~CryptManager();
};

#endif
