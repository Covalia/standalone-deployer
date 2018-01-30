#include "tools/factory/macos/macosfactory.h"
#include "tools/installpath/macos/macosinstallpathimpl.h"
#include "tools/shortcut/macos/macosshortcutimpl.h"

MacosFactory::MacosFactory()
{
}

MacosFactory::~MacosFactory()
{
}

InstallPathImpl * MacosFactory::makeInstallPath()
{
    // La classe Path est responsable de libérer cette instance
    return new MacosInstallPathImpl();
}

ShortcutImpl * MacosFactory::makeShortcut()
{
    // La classe Shortcut est responsable de libérer cette instance
    return new MacosShortcutImpl();
}
