#include "cryptmanager.h"

#include "src/utils/crypt/simplecrypt.h"

CryptManager::CryptManager()
{

}


QString CryptManager::encryptToString(const QString& text){
    SimpleCrypt crypto(Q_UINT64_C(0x0c2cd4a4bcb9f023));
    //TODO set crypt setting key
    return crypto.encryptToString(text);
}

QString CryptManager::decryptToString(const QString& text){
    SimpleCrypt crypto(Q_UINT64_C(0x0c2cd4a4bcb9f023));
    //TODO set crypt setting key
    return crypto.decryptToString(text);
}
