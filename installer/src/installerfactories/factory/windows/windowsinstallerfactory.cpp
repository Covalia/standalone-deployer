#include "installerfactories/factory/windows/windowsinstallerfactory.h"
#include "installerfactories/osresources/windows/windowsresourcesimpl.h"

WindowsInstallerFactory::WindowsInstallerFactory()
{
}

WindowsInstallerFactory::~WindowsInstallerFactory()
{
}

OsResourcesImpl * WindowsInstallerFactory::makeOsResources(const QString &_installPath)
{
    return new WindowsResourcesImpl(_installPath);
}
