#include "tools/installpath/windows/windowsinstallpathimpl.h"

WindowsInstallPathImpl::WindowsInstallPathImpl() {
}

WindowsInstallPathImpl::~WindowsInstallPathImpl() {
}

QString WindowsInstallPathImpl::getInstallationRootPath(FileSystemConfig::AppComponent _app) {
    QDir dir(QCoreApplication::applicationDirPath());
    switch (_app) {
        case FileSystemConfig::AppComponent::Loader:
            cdUp(dir, 1);
            break;
        case FileSystemConfig::AppComponent::Updater:
            cdUp(dir, 2);
            break;
    }
    return dir.absolutePath();
}
