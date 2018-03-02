#include "factories/apppath/apppath.h"
#include "factories/factory/factory.h"

AppPath::AppPath(IOConfig::AppComponent _app) : m_appPathImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_appPathImpl = QSharedPointer<AppPathImpl>(factory->makeAppPath(_app));
    }
}

AppPath::~AppPath()
{
}

QDir AppPath::getInstallationDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getInstallationDir();
    }
    return QDir();
}

void AppPath::setInstallationDir(QDir _path)
{
    if (m_appPathImpl) {
        return m_appPathImpl->setInstallationDir(_path);
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

QDir AppPath::getAppDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getAppDir();
    }
    return QDir();
}

QDir AppPath::getConfigurationDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getConfigurationDir();
    }
    return QDir();
}

QDir AppPath::getExtensionDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getExtensionDir();
    }
    return QDir();
}

QDir AppPath::getImagesDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getImagesDir();
    }
    return QDir();
}

QDir AppPath::getSlidesDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getSlidesDir();
    }
    return QDir();
}

QDir AppPath::getJavaDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getJavaDir();
    }
    return QDir();
}

QDir AppPath::getLogsDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLogsDir();
    }
    return QDir();
}

QDir AppPath::getTempDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempDir();
    }
    return QDir();
}

QDir AppPath::getCnlpDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getCnlpDir();
    }
    return QDir();
}

QDir AppPath::getDataDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getDataDir();
    }
    return QDir();
}

QDir AppPath::getUpdaterDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterDir();
    }
    return QDir();
}

QDir AppPath::getLoaderDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderDir();
    }
    return QDir();
}

QSharedPointer<QFile> AppPath::getLoaderResourcesFile()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderResourcesFile();
    }
    return QSharedPointer<QFile>(new QFile());
}

QSharedPointer<QFile> AppPath::getUpdaterResourcesFile()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterResourcesFile();
    }
    return QSharedPointer<QFile>(new QFile());
}

QSharedPointer<QFile> AppPath::getLoaderFile()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderFile();
    }
    return QSharedPointer<QFile>(new QFile());
}

QSharedPointer<QFile> AppPath::getUpdaterFile(QString updaterVersion)
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterFile(updaterVersion);
    }
    return QSharedPointer<QFile>(new QFile());
}

QSharedPointer<QFile> AppPath::getConfigurationFile()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getConfigurationFile();
    }
    return QSharedPointer<QFile>(new QFile());
}

bool AppPath::prepareLoader()
{
    if (m_appPathImpl) {
        return m_appPathImpl->prepareLoader();
    }
    return false;
}

bool AppPath::prepareUpdater(QString _version)
{
    if (m_appPathImpl) {
        return m_appPathImpl->prepareUpdater(_version);
    }
    return false;
}

bool AppPath::startLoader(QStringList _args)
{
    if (m_appPathImpl) {
        return m_appPathImpl->startLoader(_args);
    }
    return false;
}

bool AppPath::startUpdater(QString _version, QStringList _args)
{
    if (m_appPathImpl) {
        return m_appPathImpl->startUpdater(_version, _args);
    }
    return false;
}

QPair<bool, QString> AppPath::extractResource(QFile &_sourceFile, QFile &_destFile)
{
    if (m_appPathImpl) {
        return m_appPathImpl->extractResource(_sourceFile, _destFile);
    }
    QPair<bool, QString> pair = qMakePair(false, QString());
    return pair;
}

bool AppPath::makeDirectoryIfNotExists(QDir _directory, const QString &_subDir)
{
    if (m_appPathImpl) {
        return m_appPathImpl->makeDirectoryIfNotExists(_directory, _subDir);
    }
    return false;
}
