#include "tools/installpath/installpathimpl.h"
#include <QDebug>

const QString InstallPathImpl::ResourceBinPrefix(":/bin/");

InstallPathImpl::InstallPathImpl(FileSystemConfig::AppComponent _app)
{
    qDebug() << "installation directory" << m_installationDir.absolutePath() << _app;
}

InstallPathImpl::~InstallPathImpl()
{
}

QDir InstallPathImpl::getInstallationRootPath()
{
    return m_installationDir;
}

void InstallPathImpl::setInstallationRootPath(QDir _path)
{
    m_installationDir = _path;
}

bool InstallPathImpl::createDirectoryIfNotExist()
{
    QDir directory(m_installationDir);

    if (!directory.exists()) {
        return QDir().mkpath(directory.path());
    }
    return true;
}

QString InstallPathImpl::getInstallerVersion()
{
    return FileSystemConfig::InstallerVersion;
}

QString InstallPathImpl::getUpdaterVersion()
{
    return FileSystemConfig::UpdateVersion;
}

QString InstallPathImpl::getLoaderVersion()
{
    return FileSystemConfig::LoaderVersion;
}

QDir InstallPathImpl::getAppDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::AppDir));
}

QDir InstallPathImpl::getConfigurationDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ConfigurationDir));
}

QDir InstallPathImpl::getExtensionDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ExtensionDir));
}

QDir InstallPathImpl::getImagesDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ImagesDir));
}

QDir InstallPathImpl::getSlidesDirPath()
{
    return QDir(getImagesDirPath().filePath(FileSystemConfig::SlidesDir));
}

QDir InstallPathImpl::getJavaDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::JavaDir));
}

QDir InstallPathImpl::getLogsDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::LogsDir));
}

QDir InstallPathImpl::getTempDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::TempDir));
}

QDir InstallPathImpl::getUpdaterDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::UpdaterDir));
}

QDir InstallPathImpl::getLoaderDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::UpdaterDir));
}

QString InstallPathImpl::getConfigurationFilePath()
{
    return m_installationDir.absolutePath() + "/" + FileSystemConfig::ConfigurationDir + "/" + FileSystemConfig::ConfigurationFile;
}

QPair<bool, QString> InstallPathImpl::extractResourceToPath(QString resourcePath, QString copyFilePath)
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

bool InstallPathImpl::makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir)
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

bool InstallPathImpl::cdUp(QDir &_dir, int _numUp)
{
    bool result = true;

    while (_numUp > 0) {
        result &= _dir.cdUp();
        _numUp--;
    }
    return result;
}
