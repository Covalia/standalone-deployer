#include "factories/factory/macos/macosfactory.h"
#include "factories/apppath/macos/macosapppathimpl.h"
#include "factories/shortcut/macos/macosshortcutimpl.h"

MacosFactory::MacosFactory()
{
}

MacosFactory::~MacosFactory()
{
}

AppPathImpl * MacosFactory::makeAppPath(IOConfig::AppComponent _app)
{
    // La classe Path est responsable de libérer cette instance
    return new MacosAppPathImpl(_app);
}

ShortcutImpl * MacosFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new MacosShortcutImpl();
}
