#ifndef UPDATER_HASHMAC512_H
#define UPDATER_HASHMAC512_H

#include <QString>

class HashMacString : public QString {
private:
    static const int SHORT_HASHMAC_LEN = 10;
public:
    HashMacString(QString _hashmac) : QString(_hashmac) {
    }

    QString shortHashMac() const {
        return left(SHORT_HASHMAC_LEN) + "...";
    }
};

class HashMac512 {
    public:
        static HashMacString hashFromFile(const QString &_filePath, const QString &_secret);
};

#endif
