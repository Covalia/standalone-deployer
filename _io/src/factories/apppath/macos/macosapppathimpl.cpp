#include "factories/apppath/macos/macosapppathimpl.h"

MacosAppPathImpl::MacosAppPathImpl(FileSystemConfig::AppComponent _app) : AppPathImpl(_app)
{
    QDir dir(QCoreApplication::applicationDirPath());

    switch (_app) {
        case FileSystemConfig::AppComponent::Loader:
            cdUp(dir, 4);
            break;
        case FileSystemConfig::AppComponent::Updater:
            cdUp(dir, 5);
            break;
        case FileSystemConfig::AppComponent::Uninstaller:
            cdUp(dir, 3);
            break;
        case FileSystemConfig::AppComponent::Installer:
            // nothing to do, call setInstallationRootPath manually only from installer project
            break;
    }
    m_installationDir = dir.absolutePath();
}

bool MacosAppPathImpl::makeAppDirectories()
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
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::MountDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::UpdaterDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LoaderDir);

    return result;
}

QString MacosAppPathImpl::getLoaderResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::LoaderFile + FileSystemConfig::MacOSExtension;
}

QString MacosAppPathImpl::getUpdaterResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::UpdaterFile + FileSystemConfig::MacOSExtension;
}

QString MacosAppPathImpl::getLoaderFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + FileSystemConfig::MacOSExtension;
}

QString MacosAppPathImpl::getUpdaterFilePath(QString updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + updaterVersion + "/" + FileSystemConfig::UpdaterFile + FileSystemConfig::MacOSExtension;
}

bool MacosAppPathImpl::startApplication(QString _app, QStringList _args)
{
    if (!QFile::exists(_app)) {
        L_ERROR("An error occured when launching " + _app + ". The app dir doesn't exist.");
        return false;
    }

    QStringList args;
    args << "-a";
    args << _app;
    if (!_args.isEmpty()) {
        args << "--args";
        args << _args;
    }

    L_INFO("Launching app " + _app + " with args ...");
    QProcess process;
    process.start("open", args);
    // open retourne lorsque le programme est lancé, donc on peut faire un waitForFinished :)
    return process.waitForFinished();
}
