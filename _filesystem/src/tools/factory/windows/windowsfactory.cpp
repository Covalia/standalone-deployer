#include "tools/factory/windows/windowsfactory.h"
#include "tools/installpath/windows/windowsinstallpathimpl.h"

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
