#include "tools/factory/macos/macosfactory.h"
#include "tools/installpath/macos/macosinstallpathimpl.h"

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
