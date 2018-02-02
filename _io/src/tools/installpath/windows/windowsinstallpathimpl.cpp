#include "tools/installpath/windows/windowsinstallpathimpl.h"

WindowsInstallPathImpl::WindowsInstallPathImpl(FileSystemConfig::AppComponent _app) : InstallPathImpl(_app)
{
    QDir dir(QCoreApplication::applicationDirPath());

    switch (_app) {
        case FileSystemConfig::AppComponent::Loader:
            cdUp(dir, 1);
            break;
        case FileSystemConfig::AppComponent::Updater:
            cdUp(dir, 2);
            break;
        case FileSystemConfig::AppComponent::Uninstaller:
            // nothing to do
            break;
        case FileSystemConfig::AppComponent::Installer:
            // nothing to do, call setInstallationRootPath manually only from installer project
            break;
    }
    m_installationDir = dir.absolutePath();
}

bool WindowsInstallPathImpl::makeAppDirectories()
{
    bool result = true;

    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::AppDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ConfigurationDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ExtensionDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ImagesDir);
    result &= makeDirectoryIfNotExists(getImagesDirPath(), FileSystemConfig::SlidesDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::JavaDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LogsDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::TempDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::UpdaterDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LoaderDir);

    return result;
}

QString WindowsInstallPathImpl::getLoaderResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::LoaderFile + FileSystemConfig::WindowsExtension;
}

QString WindowsInstallPathImpl::getUpdaterResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::UpdaterFile + FileSystemConfig::WindowsExtension;
}

QString WindowsInstallPathImpl::getLoaderFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + FileSystemConfig::WindowsExtension;
}

QString WindowsInstallPathImpl::getUpdaterFilePath(QString updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + updaterVersion + "/" + FileSystemConfig::UpdaterFile + FileSystemConfig::WindowsExtension;
}
