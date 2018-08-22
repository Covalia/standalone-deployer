#include "secret/keymanager.h"

#include <QFile>

QString KeyManager::readHashKey()
{
    return getNamedKey(":/hash_key.txt");
}

QString KeyManager::readPasswordEncryptionKey()
{
    return getNamedKey(":/password_key.txt");
}

QString KeyManager::getNamedKey(const QString &_keyFileName)
{
    QFile file(_keyFileName);

    QString key = "";

    if (file.open(QIODevice::ReadOnly)) {
        key = file.readAll();
        file.close();
    }

    // remove whitespaces ('\t', '\n', '\v', '\f', '\r', and ' ') at start and end
    return key.trimmed();
}
