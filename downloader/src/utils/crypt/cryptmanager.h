#ifndef CRYPTMANAGER_H
#define CRYPTMANAGER_H

#include <QString>

class CryptManager
{
public:
    CryptManager();

    QString encryptToString(const QString& text);
    QString decryptToString(const QString& text);

    quint64 getKey();
};

#endif // CRYPTMANAGER_H
