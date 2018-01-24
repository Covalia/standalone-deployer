#include "fs/apptreemanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include "fs/config.h"

AppTreeManager::AppTreeManager(const QDir &_installationDir, QObject * _parent) :
    QObject(_parent),
    m_installationDir(_installationDir)
{
    qDebug() << "installation directory" << m_installationDir.absolutePath();
}

AppTreeManager::~AppTreeManager()
{
}

bool AppTreeManager::createDirectoryIfNotExist()
{
    QDir directory(m_installationDir);

    if (!directory.exists()) {
        return QDir().mkpath(directory.path());
    }
    return true;
}

bool AppTreeManager::makeAppDirectories()
{
    bool result = true;

    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::AppDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ConfigurationDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ExtensionDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ImagesDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::JavaDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LogsDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::TempDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::UpdaterDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LoaderDir);

    return result;
}

bool AppTreeManager::makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir)
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

QString AppTreeManager::getExtension()
{
    QString extension = "";

    #ifdef Q_OS_WIN
        extension = FileSystemConfig::WindowsExtension;
    #endif
    #ifdef Q_OS_MACOS
        extension = FileSystemConfig::MacOSExtension;
    #endif
    return extension;
}

QPair<bool, QString> AppTreeManager::extractResourceToPath(QString resourcePath, QString copyFilePath)
{
    if (QFile::exists(copyFilePath)) {
        QFile f(copyFilePath);
        f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool remove = f.remove();
        if (!remove) {
            QPair<bool, QString> error = qMakePair(false, "Error when remove file" + copyFilePath);
            return error;
        }
        if (QFile::exists(copyFilePath)) {
            QPair<bool, QString> error = qMakePair(false, "File still exists. Can't copie file " + resourcePath +  " in path" + copyFilePath);
            return error;
        }
    }
    bool success = QFile::copy(resourcePath, copyFilePath);
    if (success) {
        if (!QFile::exists(copyFilePath)) {
            QPair<bool, QString> error = qMakePair(false, "File don't exist. An error occured when copy file " + resourcePath +  " in path" + copyFilePath);
            return error;
        }
        QPair<bool, QString> error = qMakePair(true, "Success extracting " + resourcePath + " in " + copyFilePath);
        return error;
    } else {
        QPair<bool, QString> error = qMakePair(false, "An error occurred when copie " + resourcePath + " in " + copyFilePath);
        return error;
    }
}

QString AppTreeManager::getInstallerVersion()
{
    return FileSystemConfig::InstallerVersion;
}

QString AppTreeManager::getUpdaterVersion()
{
    return FileSystemConfig::UpdateVersion;
}

QString AppTreeManager::getLoaderVersion()
{
    return FileSystemConfig::LoaderVersion;
}

QDir AppTreeManager::getAppDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::AppDir));
}

QDir AppTreeManager::getConfigurationDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ConfigurationDir));
}

QDir AppTreeManager::getExtensionDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ExtensionDir));
}

QDir AppTreeManager::getImagesDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::ImagesDir));
}

QDir AppTreeManager::getJavaDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::JavaDir));
}

QDir AppTreeManager::getLogsDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::LogsDir));
}

QDir AppTreeManager::getTempDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::TempDir));
}

QDir AppTreeManager::getUpdaterDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::UpdaterDir));
}

QDir AppTreeManager::getLoaderDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::UpdaterDir));
}

QString AppTreeManager::getLoaderResourcesPath()
{
    return ":/bin/" + FileSystemConfig::LoaderFile + getExtension();
}

QString AppTreeManager::getUpdaterResourcesPath()
{
    return ":/bin/" + FileSystemConfig::UpdaterFile + getExtension();
}

QString AppTreeManager::getLoaderFilePath()
{
    return m_installationDir.absolutePath()+ "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + getExtension();
}

QString AppTreeManager::getUpdaterFilePath(QString updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + updaterVersion + "/" + FileSystemConfig::UpdaterFile + getExtension();
}

QString AppTreeManager:: getConfigurationFilePath()
{
    return m_installationDir.absolutePath() + "/" + FileSystemConfig::ConfigurationDir + "/" + FileSystemConfig::ConfigurationFile;
}
