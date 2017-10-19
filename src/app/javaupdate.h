#ifndef JAVAUPDATE_H
#define JAVAUPDATE_H

#include<QString>

class JavaUpdate
{

public:

    JavaUpdate(	QString _version,
                QString _file,
                QString _hash);

    QString getVersion() const;
    void setVersion(const QString &version);

    QString getFile() const;
    void setFile(const QString &file);

    QString getHash() const;
    void setHash(const QString &hash);

private:
    QString m_version;
    QString m_file;
    QString m_hash;

};

#endif // JAVAUPDATE_H
