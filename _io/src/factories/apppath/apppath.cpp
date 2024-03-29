#include "factories/apppath/apppath.h"
#include "factories/factory/factory.h"

AppPath::AppPath(IOConfig::AppComponent _app) : m_appPathImpl(nullptr)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_appPathImpl = QSharedPointer<AppPathImpl>(factory->makeAppPath(_app));
    }
}

AppPath::~AppPath()
{
}

QDir AppPath::getInstallationDir() const
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

QString AppPath::getUpdaterVersion()
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterVersion();
    }
    return "";
}

QDir AppPath::getAppDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getAppDir();
    }
    return QDir();
}

QDir AppPath::getAppOldDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getAppOldDir();
    }
    return QDir();
}

QDir AppPath::getAppNativesDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getAppNativesDir();
    }
    return QDir();
}

QDir AppPath::getConfigurationDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getConfigurationDir();
    }
    return QDir();
}

QDir AppPath::getImagesDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getImagesDir();
    }
    return QDir();
}

QDir AppPath::getSlidesDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getSlidesDir();
    }
    return QDir();
}

QDir AppPath::getJavaDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getJavaDir();
    }
    return QDir();
}

QDir AppPath::getJavaVersionDir(const QString &_version) const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getJavaVersionDir(_version);
    }
    return QDir();
}

QDir AppPath::getJavaDistDir(const QString &_version) const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getJavaDistDir(_version);
    }
    return QDir();
}

QDir AppPath::getLogsDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLogsDir();
    }
    return QDir();
}

QDir AppPath::getTempDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempDir();
    }
    return QDir();
}

QDir AppPath::getTempAppDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempAppDir();
    }
    return QDir();
}

QDir AppPath::getTempAppBuildDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempAppBuildDir();
    }
    return QDir();
}

QDir AppPath::getTempCnlpDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempCnlpDir();
    }
    return QDir();
}

QDir AppPath::getTempJavaDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempJavaDir();
    }
    return QDir();
}

QDir AppPath::getTempJavaBuildDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempJavaBuildDir();
    }
    return QDir();
}

QDir AppPath::getTempLoaderDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempLoaderDir();
    }
    return QDir();
}

QDir AppPath::getTempLoaderBuildDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempLoaderBuildDir();
    }
    return QDir();
}

QDir AppPath::getTempUpdaterDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempUpdaterDir();
    }
    return QDir();
}

QDir AppPath::getTempUpdaterBuildDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempUpdaterBuildDir();
    }
    return QDir();
}

QDir AppPath::getTempDataBuildDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getTempDataBuildDir();
    }
    return QDir();
}

QDir AppPath::getCnlpDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getCnlpDir();
    }
    return QDir();
}

QDir AppPath::getCnlpOldDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getCnlpOldDir();
    }
    return QDir();
}

QDir AppPath::getDataDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getDataDir();
    }
    return QDir();
}

QDir AppPath::getUpdaterDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterDir();
    }
    return QDir();
}

QDir AppPath::getUpdaterVersionDir(const QString &_version) const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getUpdaterVersionDir(_version);
    }
    return QDir();
}

QDir AppPath::getLoaderDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderDir();
    }
    return QDir();
}

QDir AppPath::getLoaderOldDir() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getLoaderOldDir();
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

bool AppPath::cleanAppDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanAppDir();
    }
    return false;
}

bool AppPath::cleanCnlpDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanCnlpDir();
    }
    return false;
}

bool AppPath::cleanImagesDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanImagesDir();
    }
    return false;
}

bool AppPath::cleanJavaDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanJavaDir();
    }
    return false;
}

bool AppPath::cleanLoaderDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanLoaderDir();
    }
    return false;
}

bool AppPath::cleanTempDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanTempDir();
    }
    return false;
}

bool AppPath::cleanUpdaterDir()
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanUpdaterDir();
    }
    return false;
}

bool AppPath::cleanUpdaterDirExceptVersion(const QString &_version)
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanUpdaterDirExceptVersion(_version);
    }
    return false;
}
bool AppPath::cleanJavaDirExceptVersion(const QString &_version)
{
    if (m_appPathImpl) {
        return m_appPathImpl->cleanJavaDirExceptVersion(_version);
    }
    return false;
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

bool AppPath::prepareJava(const QString &_version, bool _forceOverwrite)
{
    if (m_appPathImpl) {
        return m_appPathImpl->prepareJava(_version, _forceOverwrite);
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

bool AppPath::makeDirectoryIfNotExisting(const QDir &_directory)
{
    if (m_appPathImpl) {
        return m_appPathImpl->makeDirectoryIfNotExisting(_directory);
    }
    return false;
}

bool AppPath::startApplication(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                               const QString &_mainClass, const QString &_encoding, const QString &_dataLocation,
                               const QStringList &_arguments)
{
    if (m_appPathImpl) {
        return m_appPathImpl->startApplication(_javaVersion, _xmxMemory, _classPath, _mainClass, _encoding, _dataLocation, _arguments);
    }
    return false;
}

bool AppPath::startPostInstallTasks(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                                    const QString &_runnerClass, const QString &_encoding, const QString &_dataLocation)
{
    if (m_appPathImpl) {
        return m_appPathImpl->startPostInstallTasks(_javaVersion, _xmxMemory, _classPath,
                                                    _runnerClass, _encoding, _dataLocation);
    }
    return false;
}

QString AppPath::getClasspathSeparator() const
{
    if (m_appPathImpl) {
        return m_appPathImpl->getClasspathSeparator();
    }
    return "";
}

QString AppPath::getDataPathFromInstallPath(const QString &_installPath)
{
    return AppPathImpl::getDataPathFromInstallPath(_installPath);
}
