#include "factories/apppath/apppathimpl.h"
#include "io/fileutils.h"
#include "io/unzip/zipextractor.h"
#include <QDirIterator>
#include <QTimer>
#include <QEventLoop>

#include <QDebug>

const QString AppPathImpl::BuildDirSuffix("-build");
const QString AppPathImpl::OldDirSuffix("-old");

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

QDir AppPathImpl::getAppOldDir()
{
    return QDir(m_installationDir.filePath(IOConfig::AppDir + AppPathImpl::OldDirSuffix));
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

QDir AppPathImpl::getTempAppDir()
{
    return QDir(getTempDir().filePath(IOConfig::AppDir));
}

QDir AppPathImpl::getTempAppBuildDir()
{
    return QDir(getTempDir().filePath(IOConfig::AppDir + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getTempCnlpDir()
{
    return QDir(getTempDir().filePath(IOConfig::CnlpDir));
}

QDir AppPathImpl::getTempJavaDir()
{
    return QDir(getTempDir().filePath(IOConfig::JavaDir));
}

QDir AppPathImpl::getTempJavaBuildDir()
{
    return QDir(getTempDir().filePath(IOConfig::JavaDir + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getTempLoaderDir()
{
    return QDir(getTempDir().filePath(IOConfig::LoaderDir));
}

QDir AppPathImpl::getTempLoaderBuildDir()
{
    return QDir(getTempDir().filePath(IOConfig::LoaderDir + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getTempUpdaterDir()
{
    return QDir(getTempDir().filePath(IOConfig::UpdaterDir));
}

QDir AppPathImpl::getTempUpdaterBuildDir()
{
    return QDir(getTempDir().filePath(IOConfig::UpdaterDir  + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getCnlpDir()
{
    return QDir(m_installationDir.filePath(IOConfig::CnlpDir));
}

QDir AppPathImpl::getCnlpOldDir()
{
    return QDir(m_installationDir.filePath(IOConfig::CnlpDir + AppPathImpl::OldDirSuffix));
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
    return QDir(m_installationDir.filePath(IOConfig::LoaderDir));
}

QDir AppPathImpl::getLoaderOldDir()
{
    return QDir(m_installationDir.filePath(IOConfig::LoaderDir + AppPathImpl::OldDirSuffix));
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

bool AppPathImpl::prepareJava(const QString &_version, bool _forceOverwrite)
{
    QDir javaVersionDir = getJavaDir().absoluteFilePath(_version);

    L_INFO("Java version path: " + javaVersionDir.absolutePath());

    if (_forceOverwrite) {
        L_INFO("Force overwrite of Java dist directory: " + javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName));
        if (FileUtils::removeDirRecursively(javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName))) {
            L_INFO("Java dist directory removed.");
        } else {
            L_ERROR("Java dist directory cannot be removed.");
        }
    }

    if (javaVersionDir.exists()) {
        // the java version directory exists
        L_INFO("Java version path exists.");

        if (FileUtils::directoryExists(javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName))) {
            // directory containing an extracted version of java exists
            // nothing to do
            L_INFO("Java dist directory already exists. " + javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName));
            return true;
        } else {
            // directory containing an extracted version of java does not exist
            L_INFO("Java dist directory does not exist. " + javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName));
            if (QDir().mkpath(javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName))) {
                L_INFO("Java dist directory created: " + javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName));

                QDirIterator it(javaVersionDir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot);
                while (it.hasNext()) {
                    const QString file = javaVersionDir.relativeFilePath(it.next());
                    if (file.endsWith(".zip")) {
                        L_INFO("Found zip to extract: " + file);
                        // extract zip to dist
                        ZipExtractor zip(javaVersionDir.absoluteFilePath(file), javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName));

                        // we wait for a signal with QEventLoop and QTimer.
                        QTimer timer;
                        timer.setSingleShot(true);
                        QEventLoop loop;
                        QObject::connect(&zip, SIGNAL(finished()), &loop, SLOT(quit()));
                        QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
                        timer.start(60000);

                        // start extraction
                        zip.extract();

                        loop.exec();

                        if (timer.isActive()) {
                            if (zip.isOk()) {
                                L_INFO("OK");
                                return true;
                            } else {
                                L_ERROR("KO");
                                return false;
                            }
                        } else {
                            L_ERROR("Timeout");
                            return false;
                        }
                        break;
                    }
                }

                L_ERROR("No zip found.");
                return false;
            } else {
                L_ERROR("Unable to create Java dist directory: " + javaVersionDir.absoluteFilePath(IOConfig::JavaSubDirName));
                return false;
            }
        }
    } else {
        L_ERROR("Java version path does not exist.");
        return false;
    }

    return false;
}

bool AppPathImpl::startLoader(QStringList _args)
{
    return startComponent(getLoaderFile(), _args);
}

bool AppPathImpl::startUpdater(QString _version, QStringList _args)
{
    return startComponent(getUpdaterFile(_version), _args);
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
