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
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::JavaDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LogsDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::TempDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::UpdaterDir);

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
