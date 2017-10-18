#include "javaupdate.h"

JavaUpdate::JavaUpdate(QString _version,
                       QString _file,
                       QString _hash)
{
    version = _version;
    file = _file;
    hash = _hash;
}
