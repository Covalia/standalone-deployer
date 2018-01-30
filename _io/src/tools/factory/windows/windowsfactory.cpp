#include "tools/factory/windows/windowsfactory.h"
#include "tools/installpath/windows/windowsinstallpathimpl.h"
#include "tools/shortcut/windows/windowsshortcutimpl.h"

WindowsFactory::WindowsFactory()
{
}

WindowsFactory::~WindowsFactory()
{
}

InstallPathImpl * WindowsFactory::makeInstallPath()
{
    // La classe Path est responsable de libérer cette instance
    return new WindowsInstallPathImpl();
}

ShortcutImpl * WindowsFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new WindowsShortcutImpl();
}
