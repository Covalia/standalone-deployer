#include "factories/apppath/apppath.h"
#include "factories/factory/factory.h"

AppPath::AppPath(FileSystemConfig::AppComponent _app) : m_appPathImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_appPathImpl = QSharedPointer<AppPathImpl>(factory->makeAppPath(_app));
    }
}

AppPath::~AppPath()
{
}

QDir AppPath::getInstallationRootPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getInstallationRootPath();
    }
    return QDir();
}

void AppPath::setInstallationRootPath(QDir _path) {
    if (m_appPathImpl) {
        return m_appPathImpl->setInstallationRootPath(_path);
    }
}

bool AppPath::createDirectoryIfNotExist()
{
    if (m_appPathImpl) {
        return m_appPathImpl->createDirectoryIfNotExist();
    }
    return false;
}

bool AppPath::makeAppDirectories()
{
    if (m_appPathImpl) {
        return m_appPathImpl->makeAppDirectories();
    }
    return false;
}

QString AppPath::getInstallerVersion()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getInstallerVersion();
    }
    return "";
}

QString AppPath::getUpdaterVersion()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterVersion();
    }
    return "";
}

QString AppPath::getLoaderVersion()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderVersion();
    }
    return "";
}

QDir AppPath::getAppDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getAppDirPath();
    }
    return QDir();
}

QDir AppPath::getConfigurationDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getConfigurationDirPath();
    }
    return QDir();
}

QDir AppPath::getExtensionDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getExtensionDirPath();
    }
    return QDir();
}

QDir AppPath::getImagesDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getImagesDirPath();
    }
    return QDir();
}

QDir AppPath::getSlidesDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getSlidesDirPath();
    }
    return QDir();
}

QDir AppPath::getJavaDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getJavaDirPath();
    }
    return QDir();
}

QDir AppPath::getLogsDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLogsDirPath();
    }
    return QDir();
}

QDir AppPath::getTempDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempDirPath();
    }
    return QDir();
}

QDir AppPath::getUpdaterDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterDirPath();
    }
    return QDir();
}

QDir AppPath::getLoaderDirPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderDirPath();
    }
    return QDir();
}

QString AppPath::getLoaderResourcesPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderResourcesPath();
    }
    return "";
}

QString AppPath::getUpdaterResourcesPath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterResourcesPath();
    }
    return "";
}

QString AppPath::getLoaderFilePath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderFilePath();
    }
    return "";
}

QString AppPath::getUpdaterFilePath(QString updaterVersion)
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterFilePath(updaterVersion);
    }
    return "";
}

QString AppPath::getConfigurationFilePath()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getConfigurationFilePath();
    }
    return "";
}

QPair<bool, QString> AppPath::extractResourceToPath(QString resourcePath, QString copyFilePath)
{
    if (m_appPathImpl) {
        return m_appPathImpl->extractResourceToPath(resourcePath, copyFilePath);
    }
    QPair<bool, QString> pair = qMakePair(false, QString());
    return pair;
}

bool AppPath::makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir)
{
    if (m_appPathImpl) {
        return m_appPathImpl->makeDirectoryIfNotExists(_directoryPath, _subDir);
    }
    return false;
}
