#include "factories/apppath/apppathimpl.h"
#include "io/fileutils.h"
#include "io/unzip/zipextractor.h"
#include "log/logger.h"
#include <QDirIterator>

#include <QDebug>

const QString AppPathImpl::JavaSubDirName("dist");
const QString AppPathImpl::NativesDirName("natives");
const QString AppPathImpl::ResourceBinPrefix(":/bin/");

const QString AppPathImpl::BuildDirSuffix("-build");
const QString AppPathImpl::OldDirSuffix("-old");

AppPathImpl::AppPathImpl(IOConfig::AppComponent _app)
{
    // logger is not yet instantiated here.
    qDebug() << "Installation directory: " << m_installationDir.absolutePath() << _app;
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
    const QDir directory(m_installationDir);

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

QDir AppPathImpl::getTempDataBuildDir() const
{
    return QDir(getTempDir().absoluteFilePath(IOConfig::DataDir  + AppPathImpl::BuildDirSuffix));
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

bool AppPathImpl::cleanDir(const QString &_folder)
{
    if (FileUtils::directoryExists(_folder)) {
        L_INFO(QString("Directory to clean: %1").arg(_folder));

        if (FileUtils::removeDirRecursively(_folder)) {
            L_INFO("Removed");

            return true;
        } else {
            L_WARN("Not removed!");

            return false;
        }
    }

    return true;
}

bool AppPathImpl::cleanAppDir()
{
    return cleanDir(getAppDir().absolutePath());
}

bool AppPathImpl::cleanCnlpDir()
{
    return cleanDir(getCnlpDir().absolutePath());
}

bool AppPathImpl::cleanImagesDir()
{
    return cleanDir(getImagesDir().absolutePath());
}

bool AppPathImpl::cleanJavaDir()
{
    return cleanDir(getJavaDir().absolutePath());
}

bool AppPathImpl::cleanLoaderDir()
{
    return cleanDir(getLoaderDir().absolutePath());
}

bool AppPathImpl::cleanTempDir()
{
    return cleanDir(getTempDir().absolutePath());
}

bool AppPathImpl::cleanUpdaterDir()
{
    return cleanDir(getUpdaterDir().absolutePath());
}

bool AppPathImpl::cleanExceptVersion(const QDir &_dir, const QString &_version)
{
    L_INFO(QString("Cleaning: %1 except: %2").arg(_dir.absolutePath()).arg(_version));
    const QFileInfoList fileInfoList = _dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
    bool success = true;
    for (int i = 0; i < fileInfoList.size(); i++) {
        const QFileInfo fileInfo = fileInfoList[i];
        if (fileInfo.fileName() != _version) {
            const QString filePath = _dir.absoluteFilePath(fileInfo.fileName());
            L_INFO(QString("Cleaning: %1").arg(filePath));

            if (fileInfo.isDir()) {
                if (FileUtils::removeDirRecursively(filePath)) {
                    L_INFO("Removed");
                } else {
                    L_ERROR("Not removed.");
                    success = false;
                }
            } else if (fileInfo.isFile()) {
                QFile f(filePath);
                if (f.remove()) {
                    L_INFO("Removed");
                } else {
                    L_ERROR("Not removed.");
                    success = false;
                }
            }
        }
    }
    return success;
}

bool AppPathImpl::cleanUpdaterDirExceptVersion(const QString &_version)
{
    return cleanExceptVersion(getUpdaterDir(), _version);
}

bool AppPathImpl::cleanJavaDirExceptVersion(const QString &_version)
{
    return cleanExceptVersion(getJavaDir(), _version);
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
    const QDir javaVersionDir = getJavaVersionDir(_version);
    const QDir javaDistDir = getJavaDistDir(_version);

    L_INFO(QString("Java version path: %1").arg(javaVersionDir.absolutePath()));

    if (_forceOverwrite) {
        L_INFO(QString("Force overwrite of Java dist directory: %1").arg(javaDistDir.absolutePath()));
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
            L_INFO(QString("Java dist directory already exists: %1").arg(javaDistDir.absolutePath()));
            return true;
        } else {
            // directory containing an extracted version of java does not exist
            L_INFO(QString("Java dist directory does not exist: %1").arg(javaDistDir.absolutePath()));
            if (QDir().mkpath(javaDistDir.absolutePath())) {
                L_INFO(QString("Java dist directory created: %1").arg(javaDistDir.absolutePath()));

                QDirIterator it(javaVersionDir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot);
                while (it.hasNext()) {
                    const QString file = javaVersionDir.relativeFilePath(it.next());
                    if (file.endsWith(".zip")) {
                        L_INFO(QString("Found zip to extract: %1").arg(file));
                        // extract zip to dist
                        ZipExtractor zip(javaVersionDir.absoluteFilePath(file), javaDistDir.absolutePath());

                        // start extraction
                        zip.extract();
                        if (zip.isOk()) {
                            L_INFO(QString("%1 extracted to %2").arg(javaVersionDir.absoluteFilePath(file)).arg(javaDistDir.absolutePath()));
                            return true;
                        } else {
                            L_ERROR(QString("%1 can not be extracted to %2").arg(javaVersionDir.absoluteFilePath(file)).arg(javaDistDir.absolutePath()));
                            return false;
                        }
                    }
                }

                L_ERROR("No zip found.");
                return false;
            } else {
                L_ERROR(QString("Unable to create Java dist directory: %1").arg(javaDistDir.absolutePath()));
                return false;
            }
        }
    } else {
        L_ERROR("Java version path does not exist.");
        return false;
    }
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

bool AppPathImpl::startComponent(QSharedPointer<QFile> _app, QStringList _args)
{
    if (!_app->exists()) {
        L_ERROR(QString("An error occured when launching %1. The exe file doesn't exist.").arg(_app->fileName()));
        return false;
    }

    L_INFO(QString("Launching file %1").arg(_app->fileName()));
    QProcess process;
    return process.startDetached(_app->fileName(), _args);
}

bool AppPathImpl::startApplication(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                                   const QString &_mainClass, const QString &_encoding, const QString &_dataLocation,
                                   const QStringList &_arguments)
{
    QStringList arguments;
    const QDir installDir = getInstallationDir();

    // we won't accept zero value, so no need to test for conversion result.
    if (_xmxMemory.toInt() > 0) {
        arguments << "-Xmx" + _xmxMemory + "M";
    }
    arguments << "-Dfile.encoding=" + _encoding;
    arguments << "-Djava.library.path=" + installDir.relativeFilePath(getAppNativesDir().absolutePath());
    arguments << "-cp";
    arguments << _classPath;
    arguments << _mainClass;
    arguments << "--dataLocation=" + _dataLocation;

    // append arguments received.
    arguments << _arguments;

    const QString java_command = installDir.relativeFilePath(getJavaExecutablePath(_javaVersion));
    L_INFO(QString("Java executable: %1").arg(java_command));

    QProcess process;
    bool result = process.startDetached(java_command, arguments, installDir.absolutePath());

    if (result) {
        L_INFO("Process is started...");
    } else {
        L_ERROR(QString("Process can not start: %1").arg(process.errorString()));
    }

    return result;
}

bool AppPathImpl::startPostInstallTasks(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                                        const QString &_runnerClass, const QString &_encoding, const QString &_dataLocation)
{
    QStringList arguments;
    const QDir installDir = getInstallationDir();

    // we won't accept zero value, so no need to test for conversion result.
    if (_xmxMemory.toInt() > 0) {
        arguments << "-Xmx" + _xmxMemory + "M";
    }

    arguments << "-Dfile.encoding=" + _encoding;
    arguments << "-cp";
    arguments << _classPath;
    arguments << _runnerClass;
    arguments << getAppDir().absolutePath();

    arguments << getTempDataBuildDir().absolutePath();
    arguments << _dataLocation;

    const QString java_command = installDir.relativeFilePath(getJavaExecutablePath(_javaVersion));
    L_INFO(QString("Java executable: %1").arg(java_command));

    QProcess process;
    process.setWorkingDirectory(installDir.absolutePath());
    process.start(java_command, arguments);
    process.setReadChannel(QProcess::StandardOutput);
    bool result = process.waitForFinished();

    while (process.canReadLine()) {
        QString line = QString::fromLocal8Bit(process.readLine());
        L_INFO(line);
    }

    if (result) {
        L_INFO("Post Install Process has finished...");
    } else {
        L_ERROR(QString("Error starting Post Install Process: %1").arg(process.errorString()));
    }

    return result;
}

QString AppPathImpl::getDataPathFromInstallPath(const QString &_installPath)
{
    const QDir installDir(_installPath);

    return installDir.absoluteFilePath(IOConfig::DataDir);
}
