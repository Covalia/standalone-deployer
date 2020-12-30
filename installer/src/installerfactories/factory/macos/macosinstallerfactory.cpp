#include "installerfactories/factory/macos/macosinstallerfactory.h"
#include "installerfactories/osresources/macos/macosresourcesimpl.h"
#include "installerfactories/shortcut/macos/macosshortcutimpl.h"

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

ShortcutImpl * MacosInstallerFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new MacosShortcutImpl();
}
