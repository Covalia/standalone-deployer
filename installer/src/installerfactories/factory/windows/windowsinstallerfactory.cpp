#include "installerfactories/factory/windows/windowsinstallerfactory.h"
#include "installerfactories/osresources/windows/windowsresourcesimpl.h"
#include "installerfactories/shortcut/windows/windowsshortcutimpl.h"

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

ShortcutImpl * WindowsInstallerFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new WindowsShortcutImpl();
}
