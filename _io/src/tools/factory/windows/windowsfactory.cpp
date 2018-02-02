#include "tools/factory/windows/windowsfactory.h"
#include "tools/installpath/windows/windowsinstallpathimpl.h"
#include "tools/shortcut/windows/windowsshortcutimpl.h"

WindowsFactory::WindowsFactory()
{
}

WindowsFactory::~WindowsFactory()
{
}

InstallPathImpl * WindowsFactory::makeInstallPath(FileSystemConfig::AppComponent _app)
{
    // La classe Path est responsable de libérer cette instance
    return new WindowsInstallPathImpl(_app);
}

ShortcutImpl * WindowsFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new WindowsShortcutImpl();
}
