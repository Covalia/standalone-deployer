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

    result &= makeDirectoryIfNotExisting(getAppDir());
    result &= makeDirectoryIfNotExisting(getConfigurationDir());
    result &= makeDirectoryIfNotExisting(getImagesDir());
    result &= makeDirectoryIfNotExisting(QDir(getImagesDir().absoluteFilePath(IOConfig::SlidesDir))); // TODO
    result &= makeDirectoryIfNotExisting(getJavaDir());
    result &= makeDirectoryIfNotExisting(getLogsDir());
    result &= makeDirectoryIfNotExisting(getTempDir());
    result &= makeDirectoryIfNotExisting(getCnlpDir());
    result &= makeDirectoryIfNotExisting(getDataDir());
    result &= makeDirectoryIfNotExisting(getUpdaterDir());
    result &= makeDirectoryIfNotExisting(getLoaderDir());

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

bool WindowsAppPathImpl::startLoader(QStringList _args)
{
    return startComponent(getLoaderFile(), _args);
}

bool WindowsAppPathImpl::startUpdater(QString _version, QStringList _args)
{
    return startComponent(getUpdaterFile(_version), _args);
}

QString WindowsAppPathImpl::getJavaExecutablePath(const QString &_javaVersion) const
{
    return getJavaDistDir(_javaVersion).absoluteFilePath(QString("bin") + QDir::separator() + QString("javaw.exe"));
}

QString WindowsAppPathImpl::getClasspathSeparator() const
{
    return ";";
}

bool WindowsAppPathImpl::startComponent(QSharedPointer<QFile> _app, QStringList _args)
{
    if (!_app->exists()) {
        L_ERROR(QString("An error occured when launching %1. The exe file doesn't exist.").arg(_app->fileName()));
        return false;
    }

    const QString workingDir = m_installationDir.absolutePath();

    L_INFO(QString("Launching file %1 with working directory %2").arg(_app->fileName()).arg(workingDir));
    QProcess process;
    return process.startDetached(_app->fileName(), _args, workingDir);
}
