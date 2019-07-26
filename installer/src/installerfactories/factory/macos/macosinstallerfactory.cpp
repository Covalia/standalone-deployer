#include "installerfactories/factory/macos/macosinstallerfactory.h"
#include "installerfactories/osresources/macos/macosresourcesimpl.h"

MacosInstallerFactory::MacosInstallerFactory()
{
}

MacosInstallerFactory::~MacosInstallerFactory()
{
}

OsResourcesImpl * MacosInstallerFactory::makeOsResources(const QString &_installPath)
{
    return new MacosResourcesImpl(_installPath);
}
