#include "factories/apppath/apppathimpl.h"
#include "io/fileutils.h"
#include "io/unzip/zipextractor.h"
#include <QDirIterator>
#include <QTimer>
#include <QEventLoop>

#include <QDebug>

const QString AppPathImpl::JavaSubDirName("dist");
const QString AppPathImpl::ResourceBinPrefix(":/bin/");

const QString AppPathImpl::BuildDirSuffix("-build");
const QString AppPathImpl::OldDirSuffix("-old");
const QString AppPathImpl::NativesDirName("natives");

AppPathImpl::AppPathImpl(IOConfig::AppComponent _app)
{
    qDebug() << "installation directory" << m_installationDir.absolutePath() << _app;
}

AppPathImpl::~AppPathImpl()
{
}

QDir AppPathImpl::getInstallationDir() const
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
        return QDir().mkpath(directory.absolutePath());
    }
    return true;
}

QString AppPathImpl::getInstallerVersion()
{
    return IOConfig::InstallerVersion;
}

QString AppPathImpl::getUpdaterVersion()
{
    return IOConfig::UpdaterVersion;
}

QString AppPathImpl::getLoaderVersion()
{
    return IOConfig::LoaderVersion;
}

QDir AppPathImpl::getAppDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::AppDir));
}

QDir AppPathImpl::getAppOldDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::AppDir + AppPathImpl::OldDirSuffix));
}

QDir AppPathImpl::getAppNativesDir() const
{
    return QDir(getAppDir().absoluteFilePath(NativesDirName));
}

QDir AppPathImpl::getConfigurationDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::ConfigurationDir));
}

QDir AppPathImpl::getExtensionDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::ExtensionDir));
}

QDir AppPathImpl::getImagesDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::ImagesDir));
}

QDir AppPathImpl::getSlidesDir() const
{
    return QDir(getImagesDir().absoluteFilePath(IOConfig::SlidesDir));
}

QDir AppPathImpl::getJavaDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::JavaDir));
}

QDir AppPathImpl::getJavaVersionDir(const QString &_version) const
{
    return QDir(getJavaDir().absoluteFilePath(_version));
}

QDir AppPathImpl::getJavaDistDir(const QString &_version) const
{
    return QDir(getJavaVersionDir(_version).absoluteFilePath(AppPathImpl::JavaSubDirName));
}

QDir AppPathImpl::getLogsDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::LogsDir));
}

QDir AppPathImpl::getTempDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::TempDir));
}

QDir AppPathImpl::getTempAppDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::AppDir));
}

QDir AppPathImpl::getTempAppBuildDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::AppDir + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getTempCnlpDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::CnlpDir));
}

QDir AppPathImpl::getTempJavaDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::JavaDir));
}

QDir AppPathImpl::getTempJavaBuildDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::JavaDir + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getTempLoaderDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::LoaderDir));
}

QDir AppPathImpl::getTempLoaderBuildDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::LoaderDir + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getTempUpdaterDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::UpdaterDir));
}

QDir AppPathImpl::getTempUpdaterBuildDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::UpdaterDir  + AppPathImpl::BuildDirSuffix));
}

QDir AppPathImpl::getCnlpDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::CnlpDir));
}

QDir AppPathImpl::getCnlpOldDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::CnlpDir + AppPathImpl::OldDirSuffix));
}

QDir AppPathImpl::getDataDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::DataDir));
}

QDir AppPathImpl::getUpdaterDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::UpdaterDir));
}

QDir AppPathImpl::getUpdaterVersionDir(const QString &_version) const
{
    return QDir(getUpdaterDir().absoluteFilePath(_version));
}

QDir AppPathImpl::getLoaderDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::LoaderDir));
}

QDir AppPathImpl::getLoaderOldDir() const
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::LoaderDir + AppPathImpl::OldDirSuffix));
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

bool AppPathImpl::makeDirectoryIfNotExisting(const QDir &_directory)
{
    if (!FileUtils::directoryExists(_directory.absolutePath())) {
        return QDir().mkpath(_directory.absolutePath());
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
    Q_UNUSED(_version)
    return true;
}

bool AppPathImpl::prepareJava(const QString &_version, bool _forceOverwrite)
{
    QDir javaVersionDir = getJavaVersionDir(_version);
    QDir javaDistDir = getJavaDistDir(_version);

    L_INFO("Java version path: " + javaVersionDir.absolutePath());

    if (_forceOverwrite) {
        L_INFO("Force overwrite of Java dist directory: " + javaDistDir.absolutePath());
        if (FileUtils::removeDirRecursively(javaDistDir.absolutePath())) {
            L_INFO("Java dist directory removed.");
        } else {
            L_ERROR("Java dist directory cannot be removed.");
        }
    }

    if (javaVersionDir.exists()) {
        // the java version directory exists
        L_INFO("Java version path exists.");

        if (FileUtils::directoryExists(javaDistDir.absolutePath())) {
            // directory containing an extracted version of java exists
            // nothing to do
            L_INFO("Java dist directory already exists. " + javaDistDir.absolutePath());
            return true;
        } else {
            // directory containing an extracted version of java does not exist
            L_INFO("Java dist directory does not exist. " + javaDistDir.absolutePath());
            if (QDir().mkpath(javaDistDir.absolutePath())) {
                L_INFO("Java dist directory created: " + javaDistDir.absolutePath());

                QDirIterator it(javaVersionDir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot);
                while (it.hasNext()) {
                    const QString file = javaVersionDir.relativeFilePath(it.next());
                    if (file.endsWith(".zip")) {
                        L_INFO("Found zip to extract: " + file);
                        // extract zip to dist
                        ZipExtractor zip(javaVersionDir.absoluteFilePath(file), javaDistDir.absolutePath());

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
                L_ERROR("Unable to create Java dist directory: " + javaDistDir.absolutePath());
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
