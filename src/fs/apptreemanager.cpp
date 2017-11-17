#include "apptreemanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include "config/global.h"

AppTreeManager::AppTreeManager(const QDir &_installationDir, QObject * _parent) :
    QObject(_parent),
    m_installationDir(_installationDir)
{
    qDebug() << "installation directory" << m_installationDir.absolutePath();
}

AppTreeManager::~AppTreeManager()
{
}

bool AppTreeManager::makeAppDirectories()
{
    bool result = true;

    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, Global::AppDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, Global::ConfigurationDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, Global::ExtensionDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, Global::JavaDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, Global::LogsDir);
    result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir, Global::TempDir);

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
    return QDir(m_installationDir.filePath(Global::AppDir));
}

QDir AppTreeManager::getConfigurationDirPath()
{
    return QDir(m_installationDir.filePath(Global::ConfigurationDir));
}

QDir AppTreeManager::getExtensionDirPath()
{
    return QDir(m_installationDir.filePath(Global::ExtensionDir));
}

QDir AppTreeManager::getJavaDirPath()
{
    return QDir(m_installationDir.filePath(Global::JavaDir));
}

QDir AppTreeManager::getLogsDirPath()
{
    return QDir(m_installationDir.filePath(Global::LogsDir));
}

QDir AppTreeManager::getTempDirPath()
{
    return QDir(m_installationDir.filePath(Global::TempDir));
}
