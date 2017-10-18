#ifndef JAVAUPDATE_H
#define JAVAUPDATE_H

#include<QString>

class JavaUpdate
{
public:

    QString version;
    QString file;
    QString hash;

    JavaUpdate(	QString _version,
                QString _file,
                QString _hash);
};

#endif // JAVAUPDATE_H
