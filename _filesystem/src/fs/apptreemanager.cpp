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

QString AppTreeManager::getExtension()
{
    QString extension = "";

    #ifdef _WIN32
        extension = FileSystemConfig::WindowsExtension;
    #elif TARGET_OS_MAC
        extension = FileSystemConfig::MacOSExtension;
    #endif
    return extension;
}

QPair<bool, QString> AppTreeManager::extractResourceToPath(QString resourcePath, QString copyFilePath)
{
    if (QFile::exists(copyFilePath)) {
        QFile f(copyFilePath);
        f.setPermissions(QFile::WriteOther);
        bool remove = f.remove();
        if (!remove) {
            QPair<bool, QString> error = qMakePair(false, "Error when remove file" + copyFilePath);
            return error;
        }
    }
    bool success = QFile::copy(resourcePath, copyFilePath);
    if (success) {
        QPair<bool, QString> error = qMakePair(true, "Success extracting " + copyFilePath);
        return error;
    } else {
        QPair<bool, QString> error = qMakePair(false, "An error occurred when extracting " + copyFilePath);
        return error;
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
    return m_installationDir.absolutePath() + "/" + FileSystemConfig::LoaderFile + getExtension();
}

QString AppTreeManager::getUpdaterFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + FileSystemConfig::UpdaterFile + getExtension();
}

QString AppTreeManager:: getConfigurationFilePath(){
    return m_installationDir.absolutePath() + "/" + FileSystemConfig::ConfigurationDir + "/" + FileSystemConfig::ConfigurationFile;
}
