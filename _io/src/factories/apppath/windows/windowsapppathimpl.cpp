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
        case FileSystemConfig::AppComponent::Uninstaller:
            // nothing to do
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
    return ResourceBinPrefix + FileSystemConfig::LoaderFile + FileSystemConfig::WindowsExtension;
}

QString WindowsAppPathImpl::getUpdaterResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::UpdaterFile + FileSystemConfig::WindowsExtension;
}

QString WindowsAppPathImpl::getLoaderFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + FileSystemConfig::WindowsExtension;
}

QString WindowsAppPathImpl::getUpdaterFilePath(QString updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + updaterVersion + "/" + FileSystemConfig::UpdaterFile + FileSystemConfig::WindowsExtension;
}

bool WindowsAppPathImpl::startLoader(QStringList _args)
{
    QString loaderFile = getLoaderFilePath();
    if (!QFile::exists(loaderFile)) {
        L_ERROR("An error occured when launching Loader file " + loaderFile + ". The file doesn't exist.");
        return false;
    }

    L_INFO("Launching file " + loaderFile + " with args ...");
    QProcess process;
    return process.startDetached(loaderFile, _args);
}

bool WindowsAppPathImpl::startUpdater(QString _version, QStringList _args)
{
    QString updaterFile = getUpdaterFilePath(_version);
    if (!QFile::exists(updaterFile)) {
        L_ERROR("An error occured when launching Updater file " + updaterFile + ". The file doesn't exist.");
        return false;
    }

    L_INFO("Launching file " + updaterFile + " with args ...");
    QProcess process;
    return process.startDetached(updaterFile, _args);
}
