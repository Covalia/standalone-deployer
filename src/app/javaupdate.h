#ifndef JAVAUPDATE_H
#define JAVAUPDATE_H

#include<QString>

class JavaUpdate
{

public:

    JavaUpdate(QString _version,
               QString _file,
               QString _hash);

    QString getVersion() const;
    QString getFile() const;
    QString getHash() const;

    operator QString() const {
        return "JavaUpdate [version=" + m_version + ", file=" + m_file + ", hash=" + m_hash + "]";
    }

private:
    QString m_version;
    QString m_file;
    QString m_hash;

};

#endif
