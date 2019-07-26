#include "utils/crypt/cryptmanager.h"

#include "utils/crypt/qblowfish/qblowfish.h"

QString CryptManager::encrypt(const QString _hexKey, const QString _plaintextMessage)
{
    QBlowfish bf(QByteArray::fromHex(_hexKey.toUtf8()));

    // Enable padding to be able to encrypt an arbitrary length of bytes
    bf.setPaddingEnabled(true);

    // encrypt the message
    const QByteArray cipheredMessageBa = bf.encrypted(_plaintextMessage.toUtf8());

    // return a hex encoded of the ciphered message
    return cipheredMessageBa.toHex();
}

QString CryptManager::decrypt(const QString _hexKey, const QString _encryptedHexMessage)
{
    QBlowfish bf(QByteArray::fromHex(_hexKey.toUtf8()));

    // Enable padding to be able to encrypt an arbitrary length of bytes
    bf.setPaddingEnabled(true);

    // encrypted message must be hex encoded. decoding from hex.
    const QByteArray encryptedMessageBa = QByteArray::fromHex(_encryptedHexMessage.toUtf8());

    // decrypt message
    const QByteArray decryptedMessageBa = bf.decrypted(encryptedMessageBa);

    // convert it to string
    return QString::fromUtf8(decryptedMessageBa.constData(), decryptedMessageBa.size());
}
