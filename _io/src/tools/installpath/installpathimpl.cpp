#include "tools/installpath/installpathimpl.h"

InstallPathImpl::InstallPathImpl()
{
}

InstallPathImpl::~InstallPathImpl()
{
}

bool InstallPathImpl::cdUp(QDir &_dir, int _numUp)
{
    bool result = true;
    while (_numUp > 0) {
        result &= _dir.cdUp();
        _numUp--;
    }
    return result;
}
