#include "utils.h"

AppPath Utils::getAppPath()
{
    return AppPath(FileSystemConfig::AppComponent::Loader);
}
