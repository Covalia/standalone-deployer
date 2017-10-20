#include "javaupdate.h"

JavaUpdate::JavaUpdate(QString _version,
                       QString _file,
                       QString _hash)
{
    m_version = _version;
    m_file = _file;
    m_hash = _hash;
}

QString JavaUpdate::getVersion() const
{
    return m_version;
}

QString JavaUpdate::getFile() const
{
    return m_file;
}

QString JavaUpdate::getHash() const
{
    return m_hash;
}
