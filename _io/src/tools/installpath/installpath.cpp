#include "tools/installpath/installpath.h"
#include "tools/factory/factory.h"

InstallPath::InstallPath(FileSystemConfig::AppComponent _app) : m_pathInstallImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_pathInstallImpl = factory->makeInstallPath(_app);
    }
}

InstallPath::~InstallPath()
{
    delete m_pathInstallImpl;
}

QDir InstallPath::getInstallationRootPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getInstallationRootPath();
    }
    return QDir();
}

void InstallPath::setInstallationRootPath(QDir _path) {
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->setInstallationRootPath(_path);
    }
}

bool InstallPath::createDirectoryIfNotExist()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->createDirectoryIfNotExist();
    }
    return false;
}

bool InstallPath::makeAppDirectories()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->makeAppDirectories();
    }
    return false;
}

QString InstallPath::getInstallerVersion()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getInstallerVersion();
    }
    return "";
}

QString InstallPath::getUpdaterVersion()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getUpdaterVersion();
    }
    return "";
}

QString InstallPath::getLoaderVersion()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getLoaderVersion();
    }
    return "";
}

QDir InstallPath::getAppDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getAppDirPath();
    }
    return QDir();
}

QDir InstallPath::getConfigurationDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getConfigurationDirPath();
    }
    return QDir();
}

QDir InstallPath::getExtensionDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getExtensionDirPath();
    }
    return QDir();
}

QDir InstallPath::getImagesDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getImagesDirPath();
    }
    return QDir();
}

QDir InstallPath::getSlidesDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getSlidesDirPath();
    }
    return QDir();
}

QDir InstallPath::getJavaDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getJavaDirPath();
    }
    return QDir();
}

QDir InstallPath::getLogsDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getLogsDirPath();
    }
    return QDir();
}

QDir InstallPath::getTempDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getTempDirPath();
    }
    return QDir();
}

QDir InstallPath::getUpdaterDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getUpdaterDirPath();
    }
    return QDir();
}

QDir InstallPath::getLoaderDirPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getLoaderDirPath();
    }
    return QDir();
}

QString InstallPath::getLoaderResourcesPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getLoaderResourcesPath();
    }
    return "";
}

QString InstallPath::getUpdaterResourcesPath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getUpdaterResourcesPath();
    }
    return "";
}

QString InstallPath::getLoaderFilePath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getLoaderFilePath();
    }
    return "";
}

QString InstallPath::getUpdaterFilePath(QString updaterVersion)
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getUpdaterFilePath(updaterVersion);
    }
    return "";
}

QString InstallPath::getConfigurationFilePath()
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getConfigurationFilePath();
    }
    return "";
}

QPair<bool, QString> InstallPath::extractResourceToPath(QString resourcePath, QString copyFilePath)
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->extractResourceToPath(resourcePath, copyFilePath);
    }
    QPair<bool, QString> pair = qMakePair(false, QString());
    return pair;
}

bool InstallPath::makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir)
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->makeDirectoryIfNotExists(_directoryPath, _subDir);
    }
    return false;
}
