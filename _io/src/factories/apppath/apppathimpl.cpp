#include "factories/apppath/apppathimpl.h"
#include <QDebug>

const QString AppPathImpl::ResourceBinPrefix(":/bin/");

AppPathImpl::AppPathImpl(IOConfig::AppComponent _app)
{
    qDebug() << "installation directory" << m_installationDir.absolutePath() << _app;
}

AppPathImpl::~AppPathImpl()
{
}

QDir AppPathImpl::getInstallationDir()
{
    return m_installationDir;
}

void AppPathImpl::setInstallationDir(QDir _path)
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
    return IOConfig::InstallerVersion;
}

QString AppPathImpl::getUpdaterVersion()
{
    return IOConfig::UpdateVersion;
}

QString AppPathImpl::getLoaderVersion()
{
    return IOConfig::LoaderVersion;
}

QDir AppPathImpl::getAppDir()
{
    return QDir(m_installationDir.filePath(IOConfig::AppDir));
}

QDir AppPathImpl::getConfigurationDir()
{
    return QDir(m_installationDir.filePath(IOConfig::ConfigurationDir));
}

QDir AppPathImpl::getExtensionDir()
{
    return QDir(m_installationDir.filePath(IOConfig::ExtensionDir));
}

QDir AppPathImpl::getImagesDir()
{
    return QDir(m_installationDir.filePath(IOConfig::ImagesDir));
}

QDir AppPathImpl::getSlidesDir()
{
    return QDir(getImagesDir().filePath(IOConfig::SlidesDir));
}

QDir AppPathImpl::getJavaDir()
{
    return QDir(m_installationDir.filePath(IOConfig::JavaDir));
}

QDir AppPathImpl::getLogsDir()
{
    return QDir(m_installationDir.filePath(IOConfig::LogsDir));
}

QDir AppPathImpl::getTempDir()
{
    return QDir(m_installationDir.filePath(IOConfig::TempDir));
}

QDir AppPathImpl::getCnlpDir()
{
    return QDir(m_installationDir.filePath(IOConfig::CnlpDir));
}

QDir AppPathImpl::getDataDir()
{
    return QDir(m_installationDir.filePath(IOConfig::DataDir));
}

QDir AppPathImpl::getUpdaterDir()
{
    return QDir(m_installationDir.filePath(IOConfig::UpdaterDir));
}

QDir AppPathImpl::getLoaderDir()
{
    return QDir(m_installationDir.filePath(IOConfig::UpdaterDir));
}

QSharedPointer<QFile> AppPathImpl::getConfigurationFile()
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::ConfigurationDir + QDir::separator() + IOConfig::ConfigurationFile)));
}

QPair<bool, QString> AppPathImpl::extractResource(QFile &_sourceFile, QFile &_destFile)
{
    if (_destFile.exists()) {
        _destFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool remove = _destFile.remove();
        if (!remove) {
            QPair<bool, QString> error = qMakePair(false, "Error when removing file " + _destFile.fileName());
            return error;
        }
        if (_destFile.exists()) {
            QPair<bool, QString> error = qMakePair(false, "File still exists. Can't copy file " + _sourceFile.fileName() +  " in path" + _destFile.fileName());
            return error;
        }
    }
    bool success = _sourceFile.copy(_destFile.fileName());
    if (success) {
        if (!_destFile.exists()) {
            QPair<bool, QString> error = qMakePair(false, "File does not exist. An error occured when copying file " + _sourceFile.fileName() +  " in path" + _destFile.fileName());
            return error;
        }
        QPair<bool, QString> error = qMakePair(true, "Success extracting " + _sourceFile.fileName() + " in " + _destFile.fileName());
        return error;
    } else {
        QPair<bool, QString> error = qMakePair(false, "An error occurred when copying " + _sourceFile.fileName() + " in " + _destFile.fileName());
        return error;
    }
}

bool AppPathImpl::makeDirectoryIfNotExists(QDir _directory, const QString &_subDir)
{
    QDir directory(_directory.filePath(_subDir));

    if (_directory.exists(_subDir)) {
        // si le répertoire est en réalité un fichier, pas bon, il faut nettoyer
        // TODO check les répertoires parent ??
        _directory.remove(_subDir);
    }

    if (!directory.exists()) {
        return QDir().mkpath(directory.path());
    } else {
        return true;
    }
}

bool AppPathImpl::prepareLoader()
{
    return true;
}

bool AppPathImpl::prepareUpdater(QString _version)
{
    return true;
}

bool AppPathImpl::startLoader(QStringList _args)
{
    return startApplication(getLoaderFile(), _args);
}

bool AppPathImpl::startUpdater(QString _version, QStringList _args)
{
    return startApplication(getUpdaterFile(_version), _args);
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
