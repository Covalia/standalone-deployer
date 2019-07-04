#include "updater/appupdater.h"
#include "network/downloadmanager.h"
#include "updater/config.h"
#include "io/config.h"
#include "xml/deploymentxml.h"
#include "secret/keymanager.h"
#include "utils/hashmac/hashmac512.h"
#include "io/fileutils.h"
#include "settings/settings.h"
#include "io/unzip/zipextractor.h"
#include "settings/commandlinesingleton.h"
#include <QWidget>

#include <QDirIterator>
#include <QCoreApplication>

AppUpdater::AppUpdater(const QUrl &_appUrl, QWidget * _parent) : QObject(_parent),
    m_updater(nullptr),
    m_appPath(Utils::getAppPath()),
    m_remoteUpdaterVersion(""),
    m_localUpdaterVersion(""),
    m_remoteJavaVersion(""),
    m_localJavaVersion(""),
    m_encoding(""),
    m_mainClass(""),
    m_runnerClass(""),
    m_memory("")
{
    m_appUrl = _appUrl;

    Settings * settings = Settings::getInstance();

    QNetworkProxy proxy;
    // get proxy configuration
	if (settings->isProxyUsed()) {
		if (!settings->getProxyHostname().isEmpty()) {
			proxy.setType(QNetworkProxy::HttpProxy);
			proxy.setHostName(settings->getProxyHostname());
			proxy.setPort(static_cast<quint16>(settings->getProxyPort()));
			if (!settings->getProxyLogin().isEmpty()) {
				proxy.setUser(settings->getProxyLogin());
				if (!settings->getProxyPassword().isEmpty()) {
					proxy.setPassword(settings->getProxyPassword());
				}
			}
		}
	}

    m_updater = new DownloadManager(m_appPath.getTempDir(), _appUrl.resolved(UpdaterConfig::DeployRootPath + "/"), proxy, _parent);

    connect(m_updater, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateProgress(qint64,qint64)));
    connect(m_updater, SIGNAL(downloadSpeedUpdated(const QString&)),
            SLOT(updateDownloadSpeedMessage(const QString&)));
    connect(m_updater, SIGNAL(remainingTimeUpdated(const QString&)),
            SLOT(updateRemainingTimeMessage(const QString&)));
    connect(m_updater, SIGNAL(downloadingFileUpdated(const QString&)),
            SLOT(updateDownloadingFileMessage(const QString&)));
    connect(m_updater, SIGNAL(totalDownloadProgress(qint64,qint64)),
            SLOT(updateTotalDownloadProgress(qint64,qint64)));
    connect(m_updater, SIGNAL(proxyCredentialsChanged(const QString&,const QString&)),
            SLOT(updateProxyCredentials(const QString&,const QString&)));
    connect(m_updater, SIGNAL(errorOccurred(DownloadManagerError::ErrorType)),
            SLOT(handleDownloaderError(DownloadManagerError::ErrorType)));
}

AppUpdater::~AppUpdater()
{
    delete m_updater;
}

void AppUpdater::start()
{
    L_INFO("Updater started");
    emit serverUrlUpdated(m_appUrl);

    m_appPath.makeAppDirectories();

    QMultiMap<Application, QUrl> map;
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::AppCnlpRemoteFilename + CommandLineSingleton::getInstance()->getApplicationHttpArguments()));
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::LoaderCnlpRemoteFilename));
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::UpdaterCnlpRemoteFilename));
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::JavaCnlpRemoteFilename));

    connect(m_updater, SIGNAL(allDownloadsFinished()),
            SLOT(cnlpDownloadFinished()));

    m_updater->setUrlListToDownload(map);
}

void AppUpdater::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::updateDownloadSpeedMessage(const QString &_speed)
{
    emit downloadSpeedUpdated(_speed);
}

void AppUpdater::updateRemainingTimeMessage(const QString &_time)
{
    emit remainingTimeUpdated(_time);
}

void AppUpdater::updateDownloadingFileMessage(const QString &_file)
{
    emit downloadingFileUpdated(_file);
}

void AppUpdater::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit totalDownloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::cnlpDownloadFinished()
{
    L_INFO("CNLP files downloads finished");

    disconnect(m_updater, SIGNAL(allDownloadsFinished()),
               this, SLOT(cnlpDownloadFinished()));

    const QString loaderCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::LoaderCnlpLocalFilename);
    const QString updaterCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::UpdaterCnlpLocalFilename);
    const QString javaCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::JavaCnlpLocalFilename);
    const QString applicationCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::AppCnlpLocalFilename);
    L_INFO(QString("File to read: %1").arg(loaderCnlpPath));
    L_INFO(QString("File to read: %1").arg(updaterCnlpPath));
    L_INFO(QString("File to read: %1").arg(javaCnlpPath));
    L_INFO(QString("File to read: %1").arg(applicationCnlpPath));

    DeploymentXML loaderXml(loaderCnlpPath);
    DeploymentXML updaterXml(updaterCnlpPath);
    DeploymentXML javaXml(javaCnlpPath);
    DeploymentXML applicationXml(applicationCnlpPath);

    if (loaderXml.read() && updaterXml.read() && javaXml.read() && applicationXml.read()) {
        // retrieving remote updater version.
        m_remoteUpdaterVersion = updaterXml.getApplication().getVersion();
        L_INFO(QString("Remote Updater version: %1").arg(m_remoteUpdaterVersion));

        // retrieving application encoding
        m_encoding = applicationXml.getEncoding();
        // retrieving application main class
        m_mainClass = applicationXml.getMainClass();
        // retrieving application runner class
        m_runnerClass = applicationXml.getRunnerClass();

        // retrieving cnlp arguments
        m_arguments.clear();
        m_arguments << applicationXml.getArguments();

        // retrieving memory needed
        m_memory = applicationXml.getMemory();

        Settings * settings = Settings::getInstance();
        m_localUpdaterVersion = settings->getUpdaterVersion();
        L_INFO(QString("Local Updater version: %1").arg(m_localUpdaterVersion));
        m_localJavaVersion = settings->getJavaVersion();
        L_INFO(QString("Local Java version: %1").arg(m_localJavaVersion));

        const Application loaderApplication = Application::getLoaderApplication();
        const Application updaterApplication = Application::getUpdaterApplication();
        const Application javaApplication = Application::getJavaApplication();
        const Application appApplication = Application::getAppApplication();

        const QList<Download> loaderDownloads = loaderXml.getDownloads();
        const QList<Download> updaterDownloads = updaterXml.getDownloads();
        const QList<Download> javaDownloads = javaXml.getDownloads();
        const QList<Download> appDownloads = applicationXml.getDownloads();

        // retrieving remote java version.
        m_remoteJavaVersion = "";
        if (javaDownloads.size() > 0) {
            m_remoteJavaVersion = javaDownloads[0].getVersion();
            // only one java version for one operating system
            L_INFO(QString("Remote Java version: %1").arg(m_remoteJavaVersion));
        }

        m_cnlpParsedFiles.clear();
        {
            QListIterator<Download> iterator(loaderDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(loaderApplication, iterator.next());
            }
        }
        {
            QListIterator<Download> iterator(updaterDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(updaterApplication, iterator.next());
            }
        }
        {
            QListIterator<Download> iterator(javaDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(javaApplication, iterator.next());
            }
        }
        {
            QListIterator<Download> iterator(appDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(appApplication, iterator.next());
            }
        }

        processCnlpDownloadFileList();

        connect(m_updater, SIGNAL(allDownloadsFinished()),
                SLOT(applicationDownloadFinished()));

        const QMultiMap<Application, QString> nonAlreadyDownloadedFiles = getFilesNonAlreadyInTempDir(m_filesToDownload, m_cnlpParsedFiles);

        m_updater->setUrlListToDownload(nonAlreadyDownloadedFiles);
    } else {
        L_FATAL("Unable to read the Application cnlp files");
        emit errorOccurred(tr("Unable to read CNLP files."));
        return;
    }
}

void AppUpdater::applicationDownloadFinished()
{
    disconnect(m_updater, SIGNAL(allDownloadsFinished()),
               this, SLOT(applicationDownloadFinished()));

    L_INFO("Files successfully downloaded");

    if (checkDownloadsAreOk()) {
        // all downloads are done

        bool loaderInstalledOk = true;
        bool updaterInstalledOk = true;
        bool javaInstalledOk = true;
        bool appInstalledOk = true;

        // we install only if the download list is not empty or if the remain list is not empty.
        if (doesAppNeedToBeRebuild(Application::getLoaderApplication())) {
            L_INFO(QString("Need to rebuild and install %1").arg(Application::getLoaderApplication().getName()));
            loaderInstalledOk &= buildApplicationInTempDirectory(Application::getLoaderApplication());
            L_INFO(QString("%1 build result: %2").arg(Application::getLoaderApplication().getName()).arg(QString::number(loaderInstalledOk)));

            if (loaderInstalledOk) {
                loaderInstalledOk &= installLoader();

                if (loaderInstalledOk) {
                    L_INFO("No error reported on Loader installation.");

                    // install loader cnlp file
                    if (installCnlpFile(UpdaterConfig::LoaderCnlpLocalFilename)) {
                        L_INFO("Need a restart for Loader.");
                        // restart process
                        QStringList args = CommandLineSingleton::getInstance()->getAllArguments();
                        if (m_appPath.startLoader(args)) {
                            if (CommandLineSingleton::getInstance()->isDebugMode()) {
                                // debug mode, print args
                                L_INFO(QString("Restart Loader with: %1").arg(args.join(" ")));
                            } else {
                                L_INFO("Restart Loader...");
                            }
                            QCoreApplication::quit();
                            return;
                        } else {
                            L_FATAL("Loader restart failed.");
                            emit errorOccurred(tr("Loader restart failed."));
                            return;
                        }
                    } else {
                        L_FATAL("Error installing Loader CNLP.");
                        emit errorOccurred(tr("Error installing Loader CNLP."));
                        return;
                    }
                } else {
                    L_FATAL("Errors have been reported on Loader installation.");
                    emit errorOccurred(tr("Errors have been reported on Loader installation."));
                    return;
                }
            } else {
                L_FATAL("Errors have been reported on Loader build.");
                emit errorOccurred(tr("Errors have been reported on Loader build."));
                return;
            }
        }

        QCoreApplication::processEvents();

        if (doesAppNeedToBeRebuild(Application::getUpdaterApplication())) {
            L_INFO(QString("Need to rebuild and install %1").arg(Application::getUpdaterApplication().getName()));
            updaterInstalledOk &= buildApplicationInTempDirectory(Application::getUpdaterApplication());
            L_INFO(QString("%1 build result: %2").arg(Application::getUpdaterApplication().getName()).arg(QString::number(updaterInstalledOk)));

            if (updaterInstalledOk) {
                updaterInstalledOk &= installUpdater();

                if (updaterInstalledOk) {
                    L_INFO("No error reported on Updater installation.");

                    // install updater cnlp file
                    if (installCnlpFile(UpdaterConfig::UpdaterCnlpLocalFilename)) {
                        L_INFO("Need a restart for Updater.");
                        // restart process
                        const QStringList args = CommandLineSingleton::getInstance()->getAllArguments();
                        if (m_appPath.startLoader(args)) {
                            if (CommandLineSingleton::getInstance()->isDebugMode()) {
                                // debug mode, print args
                                L_INFO(QString("Restart Loader with: %1").arg(args.join(" ")));
                            } else {
                                L_INFO("Restart Loader...");
                            }
                            QCoreApplication::quit();
                            return;
                        } else {
                            L_FATAL("Loader restart failed.");
                            emit errorOccurred(tr("Loader restart failed."));
                            return;
                        }
                    } else {
                        L_FATAL("Error installing Updater CNLP.");
                        emit errorOccurred(tr("Error installing Updater CNLP."));
                        return;
                    }
                } else {
                    L_FATAL("Errors have been reported on Updater installation.");
                    emit errorOccurred(tr("Errors have been reported on Updater installation."));
                    return;
                }
            } else {
                L_FATAL("Errors have been reported on Updater build.");
                emit errorOccurred(tr("Errors have been reported on Updater build."));
                return;
            }
        }

        QCoreApplication::processEvents();

        if (doesAppNeedToBeRebuild(Application::getJavaApplication())) {
            L_INFO(QString("Need to rebuild and install %1").arg(Application::getJavaApplication().getName()));
            javaInstalledOk &= buildApplicationInTempDirectory(Application::getJavaApplication());
            L_INFO(QString("%1 build result: %2").arg(Application::getJavaApplication().getName()).arg(QString::number(javaInstalledOk)));

            if (javaInstalledOk) {
                javaInstalledOk &= installJava();

                if (javaInstalledOk) {
                    L_INFO("No error reported on Java installation.");

                    // install java cnlp file
                    if (!installCnlpFile(UpdaterConfig::JavaCnlpLocalFilename)) {
                        L_FATAL("Error installing Java CNLP.");
                        emit errorOccurred(tr("Error installing Java CNLP."));
                        return;
                    }
                } else {
                    L_FATAL("Errors have been reported on Java installation.");
                    emit errorOccurred(tr("Errors have been reported on Java installation."));
                    return;
                }
            } else {
                L_FATAL("Errors have been reported on Java build.");
                emit errorOccurred(tr("Errors have been reported on Java build."));
                return;
            }
        } else {
            L_INFO("Java does not need to be updated.");

            if (javaInstalledOk) {
                // dist directory was manually removed...
                const QString javaRemoteDistDir = m_appPath.getJavaDistDir(m_remoteJavaVersion).absolutePath();

                if (!FileUtils::directoryExists(javaRemoteDistDir)) {
                    L_INFO(QString("Java dist directory does not exist: %1").arg(javaRemoteDistDir));
                    if (m_appPath.prepareJava(m_remoteJavaVersion, false)) {
                        L_INFO("Java " + m_remoteJavaVersion + " soft prepared.");
                    } else {
                        L_FATAL(QString("Unable to soft prepare Java: %1").arg(m_remoteJavaVersion));
                        javaInstalledOk = false;
                        emit errorOccurred(tr("Unable to soft prepare Java: %1").arg(m_remoteJavaVersion));
                    }
                }
            }
        }

        QCoreApplication::processEvents();

        if (doesAppNeedToBeRebuild(Application::getAppApplication())) {
            L_INFO(QString("Need to rebuild and install %1").arg(Application::getAppApplication().getName()));
            appInstalledOk &= buildApplicationInTempDirectory(Application::getAppApplication());
            L_INFO(QString("%1 build result: %2").arg(Application::getAppApplication().getName()).arg(QString::number(appInstalledOk)));

            if (appInstalledOk) {
                appInstalledOk &= installApp();

                if (appInstalledOk) {
                    L_INFO("No error reported on Application installation.");

                    // install app cnlp file
                    if (!installCnlpFile(UpdaterConfig::AppCnlpLocalFilename)) {
                        L_FATAL("Error installing Application CNLP.");
                        emit errorOccurred(tr("Error installing Application CNLP."));
                        return;
                    }
                } else {
                    L_FATAL("Errors have been reported on Application installation.");
                    emit errorOccurred(tr("Errors have been reported on Application installation."));
                    return;
                }
            } else {
                L_FATAL("Errors have been reported on Application build.");
                emit errorOccurred(tr("Errors have been reported on Application build."));
                return;
            }
        }

        QCoreApplication::processEvents();

        if (loaderInstalledOk && updaterInstalledOk && javaInstalledOk && appInstalledOk) {
            // application, loader and updater didn't throw error.

            // rewrite cnlp files, in case they was removed manually
            installCnlpFile(UpdaterConfig::LoaderCnlpLocalFilename);
            installCnlpFile(UpdaterConfig::UpdaterCnlpLocalFilename);
            installCnlpFile(UpdaterConfig::JavaCnlpLocalFilename);
            installCnlpFile(UpdaterConfig::AppCnlpLocalFilename);

            bool native_extracted = true;

            const QString extractDir = m_appPath.getAppNativesDir().absolutePath();

            // optimisation: if extractDir already exists, no extraction is done
            if (!FileUtils::directoryExists(extractDir)) {
                L_INFO(QString("Natives directory %1 does not exist. Extracting...").arg(extractDir));

                // find the native jar and extract it
                QListIterator<Download> iterator(m_cnlpParsedFiles.values(Application::getAppApplication()));
                while (iterator.hasNext()) {
                    const Download & download = iterator.next();

                    if (download.isNative()) {
                        const QString fileToExtract = m_appPath.getAppDir().absoluteFilePath(download.getHref());

                        // extract zip to dist
                        ZipExtractor zip(fileToExtract, extractDir);

                        // start extraction
                        zip.extract();

                        if (zip.isOk()) {
                            L_INFO(QString("%1 extracted to %2").arg(fileToExtract).arg(extractDir));
                        } else {
                            native_extracted = false;
                            L_FATAL(QString("%1 can not be extracted to %2").arg(fileToExtract).arg(extractDir));
                            emit errorOccurred(tr("Natives can not be extracted."));
                            return;
                        }
                    }
                }
            } else {
                L_INFO(QString("Natives directory %1 already exists. No extraction done.").arg(extractDir));
            }

            QCoreApplication::processEvents();

            if (native_extracted) {
                const QString classpathSeparator = m_appPath.getClasspathSeparator();

                QString classpath = "";

                // build the classpath string
                const QDir installDir = m_appPath.getInstallationDir();
                QListIterator<Download> iterator(m_cnlpParsedFiles.values(Application::getAppApplication()));
                while (iterator.hasNext()) {
                    const Download & download = iterator.next();

                    if (download.isInClasspath() || (!download.isNative() && download.getOs() == DeploymentXML::getCurrentOsValue())) {
                        const QString fileToExtract = m_appPath.getAppDir().absoluteFilePath(download.getHref());
                        const QString relativeFile = installDir.relativeFilePath(fileToExtract);
                        L_INFO(QString("Add %1 to classpath.").arg(relativeFile));
                        if (classpath.isEmpty()) {
                            classpath += relativeFile;
                        } else {
                            classpath += classpathSeparator + relativeFile;
                        }
                    }
                }

                Settings * settings = Settings::getInstance();

                if (m_appPath.startPostInstallTasks(settings->getJavaVersion(), m_memory, classpath,
                                                    m_runnerClass, m_encoding, settings->getDataLocation())) {
                    L_INFO("Post install tasks started and finished.");

                    if (m_appPath.startApplication(settings->getJavaVersion(), m_memory, classpath, m_mainClass,
                                                   m_encoding, settings->getDataLocation(), m_arguments)) {
                        // clean temp directory
                        if (FileUtils::removeDirRecursively(m_appPath.getTempDir().absolutePath())) {
                            L_INFO(QString("Cleaned %1").arg(m_appPath.getTempDir().absolutePath()));
                        } else {
                            L_WARN(QString("Can not clean %1").arg(m_appPath.getTempDir().absolutePath()));
                        }

                        // clean old updater versions
                        L_INFO("Clean old Updater versions");
                        if (m_appPath.cleanUpdaterDirExceptVersion(m_remoteUpdaterVersion)) {
                            L_INFO("Old Updater versions cleaned");
                        } else {
                            L_WARN("Unable to clean old Updater versions");
                        }

                        // clean old java versions
                        L_INFO("Clean old Java versions");
                        if (m_appPath.cleanJavaDirExceptVersion(m_remoteJavaVersion)) {
                            L_INFO("Old Java versions cleaned");
                        } else {
                            L_WARN("Unable to clean old Java versions");
                        }

                        // quit application
                        L_INFO("Quit Application.");
                    } else {
                        L_FATAL("Unable to start Application. Exiting.");
                        emit errorOccurred(tr("Unable to start Application."));
                        return;
                    }
                } else {
                    L_FATAL("Error while running post install tasks. Exiting.");
                    emit errorOccurred(tr("Error while running post install tasks."));
                    return;
                }

                QCoreApplication::quit();
                return;
            }
        } else {
            L_FATAL("Unable to install some components. Aborting.");
            emit errorOccurred(tr("Unable to install some components."));
            return;
        }
    } else {
        L_FATAL("Error occured when downloading files. Aborting.");
        emit errorOccurred(tr("Error occured when downloading files."));
        return;
    }
}

bool AppUpdater::buildApplicationInTempDirectory(const Application &_application)
{
    bool buildOk = true;
    QDir tempAppDir;
    QDir appDir;
    QDir appBuildDir;

    if (_application == Application::getAppApplication() ||
        _application == Application::getLoaderApplication() ||
        _application == Application::getUpdaterApplication() ||
        _application == Application::getJavaApplication()) {
        if (_application == Application::getAppApplication()) {
            tempAppDir = m_appPath.getTempAppDir();
            appDir = QDir(m_appPath.getAppDir());
            appBuildDir = m_appPath.getTempAppBuildDir();
        } else if (_application == Application::getLoaderApplication()) {
            tempAppDir = m_appPath.getTempLoaderDir();
            appDir = QDir(m_appPath.getLoaderDir());
            appBuildDir = m_appPath.getTempLoaderBuildDir();
        } else if (_application == Application::getUpdaterApplication()) {
            tempAppDir = m_appPath.getTempUpdaterDir();
            appDir = m_appPath.getUpdaterVersionDir(m_remoteUpdaterVersion);
            appBuildDir = m_appPath.getTempUpdaterBuildDir();
        } else if (_application == Application::getJavaApplication()) {
            tempAppDir = m_appPath.getTempJavaDir();
            appDir = m_appPath.getJavaVersionDir(m_remoteJavaVersion);
            appBuildDir = m_appPath.getTempJavaBuildDir();
        }
    } else {
        // nothing to do, return true
        return true;
    }

    // remove temp/{application}-build directory
    L_INFO(QString("Removing previous build directory: %1").arg(appBuildDir.absolutePath()));
    FileUtils::removeDirRecursively(appBuildDir.absolutePath());

    if (QDir().mkpath(appBuildDir.absolutePath())) {
        L_INFO(QString("Creating empty build directory: %1").arg(appBuildDir.absolutePath()));

        const QList<QString> downloadedFiles = m_filesToDownload.values(_application);

        QListIterator<QString> downloadedIterator(downloadedFiles);
        while (downloadedIterator.hasNext()) {
            const QString downloadedFile = downloadedIterator.next();
            const QString localSourceFile = tempAppDir.absoluteFilePath(downloadedFile);
            const QString destinationFile = appBuildDir.absoluteFilePath(downloadedFile);

            if (QFile::exists(localSourceFile)) {
                L_INFO(QString("download source: %1").arg(localSourceFile));
                L_INFO(QString("destination:     %1").arg(destinationFile));
                QFileInfo destinationInfo(destinationFile);
                if (QDir().mkpath(destinationInfo.dir().absolutePath())) {
                    if (QFile(destinationFile).exists()) {
                        buildOk &= QFile(destinationFile).remove();
                    }
                    buildOk &= QDir().rename(localSourceFile, destinationFile);
                } else {
                    buildOk = false;
                }
            } else {
                buildOk = false;
            }
        }

        const QList<QString> keepFiles = m_filesToKeep.values(_application);

        QListIterator<QString> keepIterator(keepFiles);
        while (keepIterator.hasNext()) {
            const QString keepFile = keepIterator.next();
            const QString localSourceFile = appDir.absoluteFilePath(keepFile);
            const QString destinationFile = appBuildDir.absoluteFilePath(keepFile);

            if (QFile::exists(localSourceFile)) {
                L_INFO(QString("keep source: %1").arg(localSourceFile));
                L_INFO(QString("destination: %1").arg(destinationFile));
                QFileInfo destinationInfo(destinationFile);
                if (QDir().mkpath(destinationInfo.dir().absolutePath())) {
                    if (QFile(destinationFile).exists()) {
                        buildOk &= QFile(destinationFile).remove();
                    }
                    // we copy to not break current install
                    buildOk &= QFile(localSourceFile).copy(destinationFile);
                } else {
                    buildOk = false;
                }
            } else {
                buildOk = false;
            }
        }
    } else {
        buildOk = false;
    }

    return buildOk;
}

QList<QString> AppUpdater::getLocalFiles(const Application &_application)
{
    AppPath appPath = Utils::getAppPath();

    QList<QString> fileList;

    if (_application == Application::getAppApplication()
        || _application == Application::getLoaderApplication()
        || _application == Application::getUpdaterApplication()
        || _application == Application::getJavaApplication()) {
        QDir dir;

        if (_application == Application::getAppApplication()) {
            dir = QDir(appPath.getAppDir());
        } else if (_application == Application::getLoaderApplication()) {
            dir = QDir(appPath.getLoaderDir());
        } else if (_application == Application::getUpdaterApplication()) {
            dir = appPath.getUpdaterVersionDir(m_remoteUpdaterVersion);
        } else if (_application == Application::getJavaApplication()) {
            dir = appPath.getJavaVersionDir(m_remoteJavaVersion);
        }

        QDirIterator it(dir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            const QString filename = dir.relativeFilePath(it.next());

            // startWith tests use hardcoded "/" instead of QDir::separator() because
            // on windows, filename variable still contains "/", so tests will fail
            // if using QDir::separator()...

#ifdef Q_OS_MACOS
                // to disable false application rebuilding,
                // on macos, we ignore updater/<version>/updater.app/
                if (_application == Application::getUpdaterApplication()
                    && filename.startsWith(IOConfig::UpdaterName + IOConfig::MacOsAppExtension + "/")) {
                    L_INFO(QString("Ignoring file in %1: %2").arg(_application.getName()).arg(filename));
                    continue;
                }
                // and loader/loader.app/
                if (_application == Application::getLoaderApplication()
                    && filename.startsWith(IOConfig::LoaderName + IOConfig::MacOsAppExtension + "/")) {
                    L_INFO(QString("Ignoring file in %1: %2").arg(_application.getName()).arg(filename));
                    continue;
                }

                // and .DS_Store on macos
                if (filename.toLower().endsWith(".ds_store")) {
                    L_INFO(QString("Ignoring file in %1: %2").arg(_application.getName()).arg(filename));
                    continue;
                }
#endif

#ifdef Q_OS_WIN
                // and Thumbs.db on windows
                if (filename.toLower().endsWith("thumbs.db")) {
                    L_INFO(QString("Ignoring file in %1: %2").arg(_application.getName()).arg(filename));
                    continue;
                }
#endif

            // and java/<java-version>/dist/
            if (_application == Application::getJavaApplication()
                && filename.startsWith(AppPathImpl::JavaSubDirName + "/")) {
                L_INFO(QString("Ignoring file in %1: %2").arg(_application.getName()).arg(filename));
                continue;
            }

            // and application/natives/
            if (_application == Application::getAppApplication()
                && filename.startsWith(AppPathImpl::NativesDirName + "/")) {
                L_INFO(QString("Ignoring file in %1: %2").arg(_application.getName()).arg(filename));
                continue;
            }

            fileList.append(filename);
        }
    }

    return fileList;
}

QMultiMap<Application, QString> AppUpdater::getLocalFiles()
{
    QMultiMap<Application, QString> fileList;

    {
        const Application loaderApplication = Application::getLoaderApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getLoaderApplication()));
        while (iterator.hasNext()) {
            fileList.insert(loaderApplication, iterator.next());
        }
    }
    {
        const Application updaterApplication = Application::getUpdaterApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getUpdaterApplication()));
        while (iterator.hasNext()) {
            fileList.insert(updaterApplication, iterator.next());
        }
    }
    {
        const Application javaApplication = Application::getJavaApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getJavaApplication()));
        while (iterator.hasNext()) {
            fileList.insert(javaApplication, iterator.next());
        }
    }
    {
        const Application appApplication = Application::getAppApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getAppApplication()));
        while (iterator.hasNext()) {
            fileList.insert(appApplication, iterator.next());
        }
    }

    return fileList;
}

void AppUpdater::processCnlpDownloadFileList()
{
    // empty the lists
    m_filesToDownload.clear();
    m_filesToKeep.clear();
    m_remainingFiles.clear();

    // reading hash key only once
    const QString hash_key = KeyManager::readHashKey();

    // list of all local files (installed and in temporary directory)
    const QMultiMap<Application, QString> allLocalFiles = AppUpdater::getLocalFiles();

    // iterate over files read from cnlp files
    QSetIterator<Application> applicationIterator(QSet<Application>::fromList(m_cnlpParsedFiles.keys()));
    while (applicationIterator.hasNext()) {
        const Application & application = applicationIterator.next();

        L_INFO(QString("Processing Application: %1").arg(application.getName()));

        const QList<Download> & parsedDownloads = m_cnlpParsedFiles.values(application);

        // local files of the current application, installed only
        QList<QString> localFilesOfCurrentApplication = allLocalFiles.values(application);

        // check files to keep or download
        QListIterator<Download> parsedIterator(parsedDownloads);
        while (parsedIterator.hasNext()) {
            const Download & parsedDownload = parsedIterator.next();

            L_INFO(QString("Remote file: %1 has hash: %2").arg(parsedDownload.getHref()).arg(parsedDownload.getHashMac().shortHashMac()));

            if (localFilesOfCurrentApplication.contains(parsedDownload.getHref())) {
                QString localFile = "";
                if (application == Application::getAppApplication()) {
                    localFile = m_appPath.getAppDir().absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getLoaderApplication()) {
                    localFile = m_appPath.getLoaderDir().absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getUpdaterApplication()) {
                    localFile = m_appPath.getUpdaterVersionDir(m_remoteUpdaterVersion).absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getJavaApplication()) {
                    if (m_localJavaVersion.isEmpty()) {
                        // special case: if local version is empty, on fresh install, we search for a fake file.
                        // this prevents errors if remote version already exists locally.
                        // this should never happen unless someone manually touch config file.
                        localFile = m_appPath.getJavaVersionDir(m_remoteJavaVersion).absoluteFilePath(parsedDownload.getHref() + "-fake");
                    } else {
                        localFile = m_appPath.getJavaVersionDir(m_remoteJavaVersion).absoluteFilePath(parsedDownload.getHref());
                    }
                }

                // must be true but we test anyway
                if (QFile::exists(localFile)) {
                    const HashMacString hashmac = HashMac512::hashFromFile(localFile, hash_key);
                    L_INFO(QString("Local file: %1 has hashmac: %2").arg(localFile).arg(hashmac.shortHashMac()));

                    if (hashmac == parsedDownload.getHashMac()) {
                        L_INFO("Local and remote files are identical. Add to keep list.");
                        // add file to download list because file is out of date
                        m_filesToKeep.insert(application, parsedDownload.getHref());
                        // remove from the local list
                        localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                    } else {
                        L_INFO("Local and remote files differ. Add to download list.");
                        // add file to download list because file is out of date
                        m_filesToDownload.insert(application, parsedDownload.getHref());
                        // remove from the local list
                        localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                    }
                } else {
                    L_INFO("Local file does not exist. Add to download list.");
                    // add file to download list because it doesn't exist
                    m_filesToDownload.insert(application, parsedDownload.getHref());
                    // remove from the local list
                    localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                }
            } else {
                L_INFO("Local file does not exist. Add to download list.");
                // add file to download list because it doesn't exist
                m_filesToDownload.insert(application, parsedDownload.getHref());
                // remove from the local list
                localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
            }
        }

        // remaining files, those who no longer need to be in the app
        QListIterator<QString> iterator(localFilesOfCurrentApplication);
        while (iterator.hasNext()) {
            m_remainingFiles.insert(application, iterator.next());
        }
    }
}

bool AppUpdater::doesAppNeedToBeRebuild(const Application &_application)
{
    return !m_filesToDownload.values(_application).isEmpty()
           || !m_remainingFiles.values(_application).isEmpty();
}

bool AppUpdater::checkDownloadsAreOk() const
{
    // reading hash key only once
    const QString hash_key = KeyManager::readHashKey();

    bool downloadsOk = true;

    // check downloaded hash mac
    QMapIterator<Application, QString> iterator(m_filesToDownload);

    while (iterator.hasNext()) {
        iterator.next();
        const Application & application = iterator.key();
        const QString downloadedFile = iterator.value();

        if (application == Application::getAppApplication()
            || application == Application::getLoaderApplication()
            || application == Application::getUpdaterApplication()
            || application == Application::getJavaApplication()) {
            QDir dir;

            if (application == Application::getAppApplication()) {
                dir = QDir(m_appPath.getTempAppDir());
            } else if (application == Application::getLoaderApplication()) {
                dir = QDir(m_appPath.getTempLoaderDir());
            } else if (application == Application::getUpdaterApplication()) {
                dir = QDir(m_appPath.getTempUpdaterDir());
            } else if (application == Application::getJavaApplication()) {
                dir = QDir(m_appPath.getTempJavaDir());
            }

            // temporary location of downloaded file
            const QString localFile = dir.absoluteFilePath(downloadedFile);

            if (QFile::exists(localFile)) {
                // if this local temporary file exists
                // checking its hashmac
                const HashMacString hashmac = HashMac512::hashFromFile(localFile, hash_key);

                // we find the download corresponding to the downloadedFile to get its expected hashmac
                bool found = false;
                QListIterator<Download> parsedIterator(m_cnlpParsedFiles.values(application));
                while (parsedIterator.hasNext()) {
                    QCoreApplication::processEvents();
                    const Download & download = parsedIterator.next();
                    if (download.getHref() == downloadedFile) {
                        found = true;
                        if (hashmac != download.getHashMac()) {
                            L_WARN(QString("Bad hashmac, expected: %1, found: %2 for file: %3")
                                   .arg(download.getHashMac().shortHashMac())
                                   .arg(hashmac.shortHashMac())
                                   .arg(localFile));
                            downloadsOk = false;
                        }
                        break;
                    }
                }
                if (!found) {
                    // we print a warning if downloaded file was not found in the parsed downloads list
                    // we just log it, in case this happens, but this mustn't
                    L_WARN(QString("Downloaded file not found in the parsed downloads list: %1").arg(downloadedFile));
                }
            } else {
                // if this file does not exist, we get an error
                L_ERROR(QString("Downloaded file does not exist %1").arg(localFile));
                downloadsOk = false;
            }
        }
    }

    return downloadsOk;
}

bool AppUpdater::installLoader()
{
    // called only if loader needs to be rebuild, so we now install it.
    bool loaderInstalledOk = true;

    L_INFO(QString("Installing %1").arg(Application::getLoaderApplication().getName()));

    const QString loaderInstallDir = m_appPath.getLoaderDir().absolutePath();
    const QString loaderOldDir = m_appPath.getLoaderOldDir().absolutePath();
    const QString loaderBuildDir = m_appPath.getTempLoaderBuildDir().absolutePath();

    // remove an old loader directory if it already exists
    if (FileUtils::directoryExists(loaderOldDir)) {
        L_INFO(QString("Existing old Loader directory needs to be removed: %1").arg(loaderOldDir));

        if (FileUtils::removeDirRecursively(loaderOldDir)) {
            L_INFO(QString("Removed %1").arg(loaderOldDir));
        } else {
            L_ERROR(QString("Unable to remove %1").arg(loaderOldDir));
            loaderInstalledOk = false;
        }
    }

    if (loaderInstalledOk) {
        // we continue only if the old existing directory has been deleted

        // firstly, we rename the old app directory.
        if (QDir().rename(loaderInstallDir, loaderOldDir)) {
            L_INFO(QString("Renamed %1 to %2").arg(loaderInstallDir).arg(loaderOldDir));
            if (QDir().rename(loaderBuildDir, loaderInstallDir)) {
                L_INFO(QString("Renamed %1 to %2").arg(loaderBuildDir).arg(loaderInstallDir));
                if (FileUtils::removeDirRecursively(loaderOldDir)) {
                    L_INFO(QString("Removed %1").arg(loaderOldDir));
                } else {
                    L_WARN(QString("Unable to remove %1").arg(loaderOldDir));
                    // not an error
                }
            } else {
                L_ERROR(QString("Unable to rename %1 to %2").arg(loaderBuildDir).arg(loaderInstallDir));
                loaderInstalledOk = false;
            }
        } else {
            L_ERROR(QString("Unable to rename %1 to %2").arg(loaderInstallDir).arg(loaderOldDir));
            loaderInstalledOk = false;
        }
        // extract app from dmg (macos).
        if (m_appPath.prepareLoader()) {
            L_INFO("Loader prepared.");
        } else {
            L_ERROR("Unable to prepare Loader.");
            loaderInstalledOk = false;
        }
    }

    return loaderInstalledOk;
}

bool AppUpdater::installUpdater()
{
    // called only if updated needs to be rebuild, so we now install it.
    bool updaterInstalledOk = true;

    // we can not overwrite ourselves
    if (m_localUpdaterVersion != m_remoteUpdaterVersion) {
        L_INFO(QString("Installing %1").arg(Application::getUpdaterApplication().getName()));

        const QString updaterInstallDir = m_appPath.getUpdaterVersionDir(m_remoteUpdaterVersion).absolutePath();
        const QString updaterBuildDir = m_appPath.getTempUpdaterBuildDir().absolutePath();

        if (FileUtils::directoryExists(updaterInstallDir)) {
            // it should not exist with the previous condition, but in case it exists
            // (it is not used because local and remote versions differ), we need to remove it
            if (FileUtils::removeDirRecursively(updaterInstallDir)) {
                L_INFO(QString("Removed %1").arg(updaterInstallDir));
            } else {
                L_ERROR(QString("Unable to remove %1").arg(updaterInstallDir));
                updaterInstalledOk = false;
            }
        }

        if (updaterInstalledOk) {
            // we continue only if there is no error

            // local and remote versions differ
            if (QDir().rename(updaterBuildDir, updaterInstallDir)) {
                L_INFO(QString("Renamed %1 to %2").arg(updaterBuildDir).arg(updaterInstallDir));
                // nothing to remove. deletion will be done at next launch.
                // extract app from dmg (macos).
                if (m_appPath.prepareUpdater(m_remoteUpdaterVersion)) {
                    L_INFO(QString("Updater %1 prepared.").arg(m_remoteUpdaterVersion));
                    // write new updater version number
                    Settings * settings = Settings::getInstance();
                    settings->setUpdaterVersion(m_remoteUpdaterVersion);
                    if (settings->writeSettings()) {
                        L_INFO(QString("Written version %1 to settings.").arg(m_remoteUpdaterVersion));
                    } else {
                        L_ERROR(QString("Unable to write version %1 to settings.").arg(m_remoteUpdaterVersion));
                        updaterInstalledOk = false;
                    }
                } else {
                    L_ERROR(QString("Unable to prepare Updater %1").arg(m_remoteUpdaterVersion));
                    updaterInstalledOk = false;
                }
            } else {
                L_ERROR(QString("Unable to rename %1 to %2").arg(updaterBuildDir).arg(updaterInstallDir));
                updaterInstalledOk = false;
            }
        }
    } else {
        L_WARN("Remote and local updaters differ but have same version number, no possible update.");
    }

    return updaterInstalledOk;
}

bool AppUpdater::installJava()
{
    // called only if java needs to be rebuild, so we now install it.
    bool javaInstalledOk = true;

    // we can overwrite java because it must not be used by us.
    L_INFO(QString("Installing %1").arg(Application::getJavaApplication().getName()));

    const QString javaInstallDir = m_appPath.getJavaVersionDir(m_remoteJavaVersion).absolutePath();
    const QString javaBuildDir = m_appPath.getTempJavaBuildDir().absolutePath();

    if (FileUtils::directoryExists(javaInstallDir)) {
        // remove existing java dir if it exists
        if (FileUtils::removeDirRecursively(javaInstallDir)) {
            L_INFO(QString("Removed %1").arg(javaInstallDir));
        } else {
            L_ERROR(QString("Unable to remove %1").arg(javaInstallDir));
            javaInstalledOk = false;
        }
    }

    if (javaInstalledOk) {
        // we continue only if there is no error

        // local and remote versions differ
        if (QDir().rename(javaBuildDir, javaInstallDir)) {
            L_INFO(QString("Renamed %1 to %2").arg(javaBuildDir).arg(javaInstallDir));
            // nothing to remove. deletion will be done at next launch.
            // extract zip.
            if (m_appPath.prepareJava(m_remoteJavaVersion, true)) {
                L_INFO(QString("Java %1 force prepared.").arg(m_remoteJavaVersion));
                // write new java version number
                Settings * settings = Settings::getInstance();
                settings->setJavaVersion(m_remoteJavaVersion);
                if (settings->writeSettings()) {
                    L_INFO(QString("Written version %1 to settings.").arg(m_remoteJavaVersion));
                } else {
                    L_ERROR(QString("Unable to write version %1 to settings.").arg(m_remoteJavaVersion));
                    javaInstalledOk = false;
                }
            } else {
                L_ERROR(QString("Unable to force prepare Java %1").arg(m_remoteJavaVersion));
                javaInstalledOk = false;
            }
        } else {
            L_ERROR(QString("Unable to rename %1 to %2").arg(javaBuildDir).arg(javaInstallDir));
            javaInstalledOk = false;
        }
    }

    return javaInstalledOk;
}

bool AppUpdater::installApp()
{
    // called only if app needs to be rebuild, so we now install it.
    bool appInstalledOk = true;

    L_INFO(QString("Installing %1").arg(Application::getAppApplication().getName()));

    const QString appInstallDir = m_appPath.getAppDir().absolutePath();
    const QString appOldDir = m_appPath.getAppOldDir().absolutePath();
    const QString appBuildDir = m_appPath.getTempAppBuildDir().absolutePath();

    // remove an old app directory if it already exists
    if (FileUtils::directoryExists(appOldDir)) {
        L_INFO(QString("Existing old Application directory needs to be removed: %1").arg(appOldDir));

        if (FileUtils::removeDirRecursively(appOldDir)) {
            L_INFO(QString("Removed %1").arg(appOldDir));
        } else {
            L_ERROR(QString("Unable to remove %1").arg(appOldDir));
            appInstalledOk = false;
        }
    }

    if (appInstalledOk) {
        // we continue only if the old existing directory has been deleted

        // firstly, we rename the old app directory.
        if (QDir().rename(appInstallDir, appOldDir)) {
            L_INFO(QString("Renamed %1 to %2").arg(appInstallDir).arg(appOldDir));
            if (QDir().rename(appBuildDir, appInstallDir)) {
                L_INFO(QString("Renamed %1 to %2").arg(appBuildDir).arg(appInstallDir));
                if (FileUtils::removeDirRecursively(appOldDir)) {
                    L_INFO(QString("Removed %1").arg(appOldDir));
                } else {
                    L_WARN(QString("Unable to remove %1").arg(appOldDir));
                    // not an error
                }
            } else {
                L_ERROR(QString("Unable to rename %1 to %2").arg(appBuildDir).arg(appInstallDir));
                appInstalledOk = false;
            }
        } else {
            L_ERROR(QString("Unable to rename %1 to %2").arg(appInstallDir).arg(appOldDir));
            appInstalledOk = false;
        }
    }

    return appInstalledOk;
}

bool AppUpdater::installCnlpFile(const QString &_file)
{
    const QString cnlpInstallFile = m_appPath.getCnlpDir().absoluteFilePath(_file);
    const QString cnlpBuildFile = m_appPath.getTempCnlpDir().absoluteFilePath(_file);

    bool cnlpInstalledOk = true;

    if (QFile::exists(cnlpInstallFile)) {
        L_INFO(QString("%1 already exists. Removing.").arg(cnlpInstallFile));
        if (QFile::remove(cnlpInstallFile)) {
            L_INFO(QString("%1 removed.").arg(cnlpInstallFile));
        } else {
            L_ERROR(QString("Unable to remove %1").arg(cnlpInstallFile));
            cnlpInstalledOk = false;
        }
    }

    if (cnlpInstalledOk) {
        if (QFile::copy(cnlpBuildFile, cnlpInstallFile)) {
            L_INFO(QString("Copy %1 to %2").arg(cnlpBuildFile).arg(cnlpInstallFile));
        } else {
            L_ERROR(QString("Unable to copy %1 to %2").arg(cnlpBuildFile).arg(cnlpInstallFile));
            cnlpInstalledOk = false;
        }
    }

    return cnlpInstalledOk;
}

QMultiMap<Application, QString> AppUpdater::getFilesNonAlreadyInTempDir(const QMultiMap<Application, QString> _fullDownloadMap,
                                                                        const QMultiMap<Application, Download> _cnlpParsedFiles)
{
    // copy list
    QMultiMap<Application, QString> outMap = _fullDownloadMap;

    // reading hash key only once
    const QString hash_key = KeyManager::readHashKey();

    // check downloaded hash mac
    QMutableMapIterator<Application, QString> iterator(outMap);

    while (iterator.hasNext()) {
        iterator.next();
        const Application & application = iterator.key();
        const QString downloadedFile = iterator.value();

        if (application == Application::getAppApplication()
            || application == Application::getLoaderApplication()
            || application == Application::getUpdaterApplication()
            || application == Application::getJavaApplication()) {
            QDir dir;

            const AppPath appPath = Utils::getAppPath();

            if (application == Application::getAppApplication()) {
                dir = QDir(appPath.getTempAppDir());
            } else if (application == Application::getLoaderApplication()) {
                dir = QDir(appPath.getTempLoaderDir());
            } else if (application == Application::getUpdaterApplication()) {
                dir = QDir(appPath.getTempUpdaterDir());
            } else if (application == Application::getJavaApplication()) {
                dir = QDir(appPath.getTempJavaDir());
            }

            // temporary location of downloaded file
            const QString localFile = dir.absoluteFilePath(downloadedFile);

            if (QFile::exists(localFile)) {
                // if this local temporary file exists
                // checking its hashmac
                const HashMacString hashmac = HashMac512::hashFromFile(localFile, hash_key);

                // we find the download corresponding to the downloadedFile to get its expected hashmac
                QListIterator<Download> parsedIterator(_cnlpParsedFiles.values(application));
                while (parsedIterator.hasNext()) {
                    const Download & download = parsedIterator.next();
                    if (download.getHref() == downloadedFile) {
                        if (hashmac == download.getHashMac()) {
                            L_INFO(QString("File for Application: %1 already downloaded: %2").arg(application.getName()).arg(download.getHref()));
                            // remove this from temp list
                            iterator.remove();
                        }
                        break;
                    }
                }
            }
        }
    }

    return outMap;
}

void AppUpdater::updateProxyCredentials(const QString &_login, const QString &_password)
{
    Settings * settings = Settings::getInstance();

    settings->setProxyLogin(_login);
    settings->setProxyPassword(_password);

    settings->writeSettings();
}

void AppUpdater::handleDownloaderError(DownloadManagerError::ErrorType _error)
{
    switch (_error) {
        case DownloadManagerError::ErrorType::ProxyAuthenticationError:
            L_FATAL(QString("Authentication error with proxy."));
            emit errorOccurred(tr("The proxy has not been able to authenticate the user."));
            break;

        case DownloadManagerError::ErrorType::DownloadError:
            L_FATAL(QString("Download error."));
            emit errorOccurred(tr("Errors occurred while downloading files."));
            break;

        case DownloadManagerError::ErrorType::TimeoutError:
            L_FATAL(QString("Timeout error."));
            emit errorOccurred(tr("A timeout occurred while downloading files."));
            break;
    }
}
