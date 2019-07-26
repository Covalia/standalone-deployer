#include "utils.h"

AppPath Utils::getAppPath()
{
    return AppPath(IOConfig::AppComponent::Loader);
}
