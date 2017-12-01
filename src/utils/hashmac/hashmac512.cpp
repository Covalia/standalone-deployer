#include "utils/hashmac/hashmac512.h"
#include <QFile>
#include <QMessageAuthenticationCode>

QString HashMac512::hashFromFile(const QString &_filePath, const QString &_secret)
{
    QFile file(_filePath);

    if (file.open(QIODevice::ReadOnly)) {
        const QByteArray content = file.readAll();
        file.close();

        return QMessageAuthenticationCode::hash(content, _secret.toUtf8(), QCryptographicHash::Sha512).toHex();
    }

    return "";
}
