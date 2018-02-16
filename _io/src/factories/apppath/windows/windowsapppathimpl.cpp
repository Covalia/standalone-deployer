#include "factories/apppath/windows/windowsapppathimpl.h"

WindowsAppPathImpl::WindowsAppPathImpl(FileSystemConfig::AppComponent _app) : AppPathImpl(_app)
{
    QDir dir(QCoreApplication::applicationDirPath());

    switch (_app) {
        case FileSystemConfig::AppComponent::Loader:
            cdUp(dir, 1);
            break;
        case FileSystemConfig::AppComponent::Updater:
            cdUp(dir, 2);
            break;
        case FileSystemConfig::AppComponent::Installer:
            // nothing to do, call setInstallationRootPath manually only from installer project
            break;
    }
    m_installationDir = dir.absolutePath();
}

bool WindowsAppPathImpl::makeAppDirectories()
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

QString WindowsAppPathImpl::getLoaderResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::LoaderFile + FileSystemConfig::WindowsAppExtension;
}

QString WindowsAppPathImpl::getUpdaterResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::UpdaterFile + FileSystemConfig::WindowsAppExtension;
}

QString WindowsAppPathImpl::getLoaderFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + FileSystemConfig::WindowsAppExtension;
}

QString WindowsAppPathImpl::getUpdaterFilePath(QString _updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + _updaterVersion + "/" + FileSystemConfig::UpdaterFile + FileSystemConfig::WindowsAppExtension;
}

bool WindowsAppPathImpl::startApplication(QString _app, QStringList _args)
{
    if (!QFile::exists(_app)) {
        L_ERROR("An error occured when launching " + _app + ". The exe file doesn't exist.");
        return false;
    }

    L_INFO("Launching file " + _app);
    QProcess process;
    return process.startDetached(_app, _args);
}
