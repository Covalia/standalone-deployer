#include "utils.h"
#include "tools/installpath/installpath.h"

QString Utils::getInstallationRootPath()
{
    InstallPath p;

    return p.getInstallationRootPath(FileSystemConfig::AppComponent::Uninstaller);
}
