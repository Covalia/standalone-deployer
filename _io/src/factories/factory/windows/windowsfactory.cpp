#include "factories/factory/windows/windowsfactory.h"
#include "factories/apppath/windows/windowsapppathimpl.h"
#include "factories/shortcut/windows/windowsshortcutimpl.h"

WindowsFactory::WindowsFactory()
{
}

WindowsFactory::~WindowsFactory()
{
}

AppPathImpl * WindowsFactory::makeAppPath(FileSystemConfig::AppComponent _app)
{
    // La classe Path est responsable de libérer cette instance
    return new WindowsAppPathImpl(_app);
}

ShortcutImpl * WindowsFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new WindowsShortcutImpl();
}
