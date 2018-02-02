#include "tools/installpath/macos/macosinstallpathimpl.h"

#include <QDir>

MacosInstallPathImpl::MacosInstallPathImpl()
{
}

MacosInstallPathImpl::~MacosInstallPathImpl()
{
}

QString MacosInstallPathImpl::getInstallationRootPath(FileSystemConfig::AppComponent _app)
{
    QDir dir(QCoreApplication::applicationDirPath());
    switch (_app) {
        case FileSystemConfig::AppComponent::Loader:
            cdUp(dir, 4);
            break;
        case FileSystemConfig::AppComponent::Updater:
            cdUp(dir, 5);
            break;
    }
    return dir.absolutePath();
}
