#include "cryptmanager.h"

#include "utils/crypt/simplecrypt.h"
#include "settings/resourcessettings.h"
#include "log/logger.h"

CryptManager::CryptManager()
{
}

QString CryptManager::encryptToString(const QString& text)
{
    SimpleCrypt crypto(getKey());

    // TODO set crypt setting key
    return crypto.encryptToString(text);
}

QString CryptManager::decryptToString(const QString& text)
{
    SimpleCrypt crypto(getKey());

    // TODO set crypt setting key
    return crypto.decryptToString(text);
}


quint64 CryptManager::getKey(){
    ResourcesSettings * resource = ResourcesSettings::getInstance();
    QString key = resource->getEncrypted_password_key();
    bool ok;
    quint64 quintKey = key.toLongLong(&ok,16);
    if(ok){
        return quintKey;
    }
    else{
        L_ERROR("Error in converting encrypted key to quint64, please change key in resources project");
        return Q_UINT64_C(0x0c2cd4a4bcb9f023);
    }
}
