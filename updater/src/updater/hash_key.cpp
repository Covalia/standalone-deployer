#include "updater/hash_key.h"

#include <QFile>

QString HashKey::readHashKey() {

    QString filename(":/hash_key.txt");
    QFile file(filename);

    QString hash = "";

    if (file.open(QIODevice::ReadOnly)) {
        hash = file.readAll();
        file.close();
    }

    // remove whitespaces ('\t', '\n', '\v', '\f', '\r', and ' ') at start and end
    return hash.trimmed();

}
