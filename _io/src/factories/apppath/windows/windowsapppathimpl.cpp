#include "factories/apppath/windows/windowsapppathimpl.h"

WindowsAppPathImpl::WindowsAppPathImpl(IOConfig::AppComponent _app) : AppPathImpl(_app)
{
    QDir dir(QCoreApplication::applicationDirPath());

    switch (_app) {
        case IOConfig::AppComponent::Loader:
            cdUp(dir, 1);
            break;
        case IOConfig::AppComponent::Updater:
            cdUp(dir, 2);
            break;
        case IOConfig::AppComponent::Installer:
            // nothing to do, call setInstallationDir manually only from installer project
            break;
    }
    m_installationDir = dir.absolutePath();
}

bool WindowsAppPathImpl::makeAppDirectories()
{
    bool result = true;

    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::AppDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::ConfigurationDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::ExtensionDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::ImagesDir);
    result &= makeDirectoryIfNotExists(getImagesDir(), IOConfig::SlidesDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::JavaDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::LogsDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::TempDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::CnlpDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::DataDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::UpdaterDir);
    result &= makeDirectoryIfNotExists(m_installationDir, IOConfig::LoaderDir);

    return result;
}

QSharedPointer<QFile> WindowsAppPathImpl::getLoaderResourcesFile()
{
    return QSharedPointer<QFile>(new QFile(ResourceBinPrefix + IOConfig::LoaderFile + IOConfig::WindowsAppExtension));
}

QSharedPointer<QFile> WindowsAppPathImpl::getUpdaterResourcesFile()
{
    return QSharedPointer<QFile>(new QFile(ResourceBinPrefix + IOConfig::UpdaterFile + IOConfig::WindowsAppExtension));
}

QSharedPointer<QFile> WindowsAppPathImpl::getLoaderFile()
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::LoaderDir + QDir::separator() + IOConfig::LoaderFile + IOConfig::WindowsAppExtension)));
}

QSharedPointer<QFile> WindowsAppPathImpl::getUpdaterFile(QString _updaterVersion)
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::UpdaterDir + QDir::separator() + _updaterVersion + QDir::separator() + IOConfig::UpdaterFile + IOConfig::WindowsAppExtension)));
}

bool WindowsAppPathImpl::startApplication(QSharedPointer<QFile> _app, QStringList _args)
{
    if (!_app->exists()) {
        L_ERROR("An error occured when launching " + _app->fileName() + ". The exe file doesn't exist.");
        return false;
    }

    L_INFO("Launching file " + _app->fileName());
    QProcess process;
    return process.startDetached(_app->fileName(), _args);
}
