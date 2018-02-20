#include "utils.h"

#include <QDir>

AppPath Utils::getAppPath()
{
    return AppPath(IOConfig::AppComponent::Installer);
}

QString Utils::getInstallerlLogPath()
{
    // log into home directory while installing
    QDir installerParentDir(QDir::homePath());

    return installerParentDir.absoluteFilePath("installer.log");
}
