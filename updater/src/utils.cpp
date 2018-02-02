#include "utils.h"

InstallPath Utils::getInstallPath()
{
    return InstallPath(FileSystemConfig::AppComponent::Updater);
}
