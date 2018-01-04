#ifndef SETTINGS__CRYPTMANAGER_H
#define SETTINGS__CRYPTMANAGER_H

#include <QString>

class CryptManager
{
    public:
        CryptManager();

        QString encryptToString(const QString &text);
        QString decryptToString(const QString &text);

        quint64 getKey();
};

#endif // SETTINGS__CRYPTMANAGER_H
