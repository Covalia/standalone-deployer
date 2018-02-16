#include "factories/apppath/apppathimpl.h"
#include <QDebug>

const QString AppPathImpl::ResourceBinPrefix(":/bin/");

AppPathImpl::AppPathImpl(FileSystemConfig::AppComponent _app)
{
    qDebug() << "installation directory" << m_installationDir.absolutePath() << _app;
}

AppPathImpl::~AppPathImpl()
{
}

QDir AppPathImpl::getInstallationRootPath()
{
    return m_installationDir;
}

void AppPathImpl::setInstallationRootPath(QDir _path)
{
    m_installationDir = _path;
}

bool AppPathImpl::createDirectoryIfNotExist()
{
    QDir directory(m_installationDir);

    if (!directory.exists()) {
        return QDir().mkpath(directory.path());
    }
    return true;
}

QString AppPathImpl::getInstallerVersion()
{
    return FileSystemConfig::InstallerVersion;
}

QString AppPathImpl::getUpdaterVersion()
{
    return FileSystemConfig::UpdateVersion;
}

QString AppPathImpl::getLoaderVersion()
{
    return FileSystemConfig::LoaderVersion;
}

QDir AppPathImpl::getAppDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::AppDir));
}

QDir AppPathImpl::getConfigurationDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ConfigurationDir));
}

QDir AppPathImpl::getExtensionDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ExtensionDir));
}

QDir AppPathImpl::getImagesDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ImagesDir));
}

QDir AppPathImpl::getSlidesDirPath()
{
    return QDir(getImagesDirPath().filePath(FileSystemConfig::SlidesDir));
}

QDir AppPathImpl::getJavaDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::JavaDir));
}

QDir AppPathImpl::getLogsDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::LogsDir));
}

QDir AppPathImpl::getTempDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::TempDir));
}

QDir AppPathImpl::getUpdaterDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::UpdaterDir));
}

QDir AppPathImpl::getLoaderDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::UpdaterDir));
}

QString AppPathImpl::getConfigurationFilePath()
{
    return m_installationDir.absolutePath() + "/" + FileSystemConfig::ConfigurationDir + "/" + FileSystemConfig::ConfigurationFile;
}

QPair<bool, QString> AppPathImpl::extractResourceToPath(QString resourcePath, QString copyFilePath)
{
    if (QFile::exists(copyFilePath)) {
        QFile f(copyFilePath);
        f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool remove = f.remove();
        if (!remove) {
            QPair<bool, QString> error = qMakePair(false, "Error when removing file " + copyFilePath);
            return error;
        }
        if (QFile::exists(copyFilePath)) {
            QPair<bool, QString> error = qMakePair(false, "File still exists. Can't copy file " + resourcePath +  " in path" + copyFilePath);
            return error;
        }
    }
    bool success = QFile::copy(resourcePath, copyFilePath);
    if (success) {
        if (!QFile::exists(copyFilePath)) {
            QPair<bool, QString> error = qMakePair(false, "File don't exist. An error occured when copying file " + resourcePath +  " in path" + copyFilePath);
            return error;
        }
        QPair<bool, QString> error = qMakePair(true, "Success extracting " + resourcePath + " in " + copyFilePath);
        return error;
    } else {
        QPair<bool, QString> error = qMakePair(false, "An error occurred when copying " + resourcePath + " in " + copyFilePath);
        return error;
    }
}

bool AppPathImpl::makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir)
{
    QDir directory(_directoryPath.filePath(_subDir));

    if (_directoryPath.exists(_subDir)) {
        // si le répertoire est en réalité un fichier, pas bon, il faut nettoyer
        // TODO check les répertoires parent ??
        _directoryPath.remove(_subDir);
    }

    if (!directory.exists()) {
        return QDir().mkpath(directory.path());
    } else {
        return true;
    }
}

bool AppPathImpl::startLoader(QStringList _args)
{
    return startApplication(getLoaderFilePath(), _args);
}

bool AppPathImpl::startUpdater(QString _version, QStringList _args)
{
    return startApplication(getUpdaterFilePath(_version), _args);
}

bool AppPathImpl::cdUp(QDir &_dir, int _numUp)
{
    bool result = true;

    while (_numUp > 0) {
        result &= _dir.cdUp();
        _numUp--;
    }
    return result;
}
