#include "updater/appupdater.h"
#include "network/downloadmanager.h"
#include "updater/config.h"
#include "io/config.h"
#include "xml/deploymentxml.h"
#include "updater/hash_key.h"
#include "utils/hashmac/hashmac512.h"
#include "io/fileutils.h"
#include "settings/settings.h"
#include "utils/unzip/zipextractor.h"

#include <QDirIterator>
#include <QTimer>
#include <QEventLoop>

AppUpdater::AppUpdater(const QUrl &_appUrl, const QDir &_appInstallDir, QObject * _parent) : QObject(_parent),
    m_updater(0),
    m_appPath(Utils::getAppPath()),
    m_remoteUpdaterVersion(""),
    m_localUpdaterVersion(""),
    m_remoteJavaVersion(""),
    m_localJavaVersion("")
{
    m_appUrl = _appUrl;

    QNetworkProxy proxy;
    // TODO récupérer ici la configuration du proxy.
    // proxy.setType(QNetworkProxy::HttpProxy);
    // proxy.setHostName("10.33.26.250");
    // proxy.setPort(3129);
    // proxy.setUser("user");
    // proxy.setPassword("password");

    m_updater = new DownloadManager(m_appPath.getTempDir(), _appUrl.resolved(UpdaterConfig::DeployRootPath + "/"), proxy, this);

    connect(m_updater, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateProgress(qint64,qint64)));
    connect(m_updater, SIGNAL(downloadSpeedMessage(const QString&)),
            SLOT(updateDownloadSpeedMessage(const QString&)));
    connect(m_updater, SIGNAL(remainingTimeMessage(const QString&)),
            SLOT(updateRemainingTimeMessage(const QString&)));
    connect(m_updater, SIGNAL(downloadFileMessage(const QString&)),
            SLOT(updateDownloadFileMessage(const QString&)));
    connect(m_updater, SIGNAL(totalDownloadProgress(qint64,qint64)),
            SLOT(updateTotalDownloadProgress(qint64,qint64)));
}

AppUpdater::~AppUpdater()
{
    delete m_updater;
}

void AppUpdater::start()
{
    L_INFO("Updater started");
    emit serverUrlMessage(m_appUrl);

    bool result = m_appPath.makeAppDirectories();

    QList<QUrl> downloads;
    // TODO disable progress bar for cnlp files
    downloads << QUrl(UpdaterConfig::AppCnlpRemoteFilename);
    downloads << QUrl(UpdaterConfig::LoaderCnlpRemoteFilename);
    downloads << QUrl(UpdaterConfig::UpdaterCnlpRemoteFilename);
    downloads << QUrl(UpdaterConfig::JavaCnlpRemoteFilename);

    QMap<Application, QList<QUrl> > map;
    map.insert(Application::getCnlpApplication(), downloads);

    connect(m_updater, SIGNAL(downloadsFinished()),
            SLOT(cnlpDownloadFinished()));

    m_updater->setUrlListToDownload(map);
}

void AppUpdater::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::updateDownloadSpeedMessage(const QString &_speed)
{
    emit downloadSpeedMessage(_speed);
}

void AppUpdater::updateRemainingTimeMessage(const QString &_time)
{
    emit remainingTimeMessage(_time);
}

void AppUpdater::updateDownloadFileMessage(const QString &_file)
{
    emit downloadFileMessage(_file);
}

void AppUpdater::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit totalDownloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::cnlpDownloadFinished()
{
    L_INFO("CNLP files downloads finished");

    disconnect(m_updater, SIGNAL(downloadsFinished()),
               this, SLOT(cnlpDownloadFinished()));

    const QString applicationCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::AppCnlpLocalFilename);
    const QString updaterCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::UpdaterCnlpLocalFilename);
    const QString loaderCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::LoaderCnlpLocalFilename);
    const QString javaCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::JavaCnlpLocalFilename);
    L_INFO("File to read: " + applicationCnlpPath);
    L_INFO("File to read: " + updaterCnlpPath);
    L_INFO("File to read: " + loaderCnlpPath);
    L_INFO("File to read: " + javaCnlpPath);

    DeploymentXML applicationXml(applicationCnlpPath);
    DeploymentXML updaterXml(updaterCnlpPath);
    DeploymentXML loaderXml(loaderCnlpPath);
    DeploymentXML javaXml(javaCnlpPath);

    if (applicationXml.read() && updaterXml.read() && loaderXml.read() && javaXml.read()) {
        // retrieving remote updater version.
        QMap<Application, QList<Download> >::key_iterator iterator = updaterXml.getApplications().keyBegin();
        while (iterator != updaterXml.getApplications().keyEnd()) {
            const Application app = *iterator;
            if (app.getName() == Application::getUpdaterApplication().getName()) {
                m_remoteUpdaterVersion = app.getVersion();
            }
            ++iterator;
        }

        Settings * settings = Settings::getInstance();
        m_localUpdaterVersion = settings->getUpdaterVersion();
        m_localJavaVersion = settings->getJavaVersion();

        const Application appApplication = Application::getAppApplication();
        const Application updaterApplication = Application::getUpdaterApplication();
        const Application loaderApplication = Application::getLoaderApplication();
        const Application javaApplication = Application::getJavaApplication();

        // TODO refactor into an helper class?
        QString osValue;
#ifdef Q_OS_MACOS
            osValue = DeploymentXML::OsMacOsValue;
#endif

#ifdef Q_OS_WIN
            osValue = DeploymentXML::OsWindowsValue;
#endif
        const QList<Download> appDownloads = applicationXml.getDownloads(appApplication, osValue);
        const QList<Download> updaterDownloads = updaterXml.getDownloads(updaterApplication, osValue);
        const QList<Download> loaderDownloads = loaderXml.getDownloads(loaderApplication, osValue);
        const QList<Download> javaDownloads = javaXml.getDownloads(javaApplication, osValue);

        // retrieving remote java version.
        m_remoteJavaVersion = "";
        for (int i = 0; i < javaDownloads.size(); ++i) {
            m_remoteJavaVersion = javaDownloads[i].getVersion();
            // only one java version for one operating system
            break;
        }

        m_cnlpParsedFiles.clear();
        m_cnlpParsedFiles.insert(appApplication, appDownloads);
        m_cnlpParsedFiles.insert(updaterApplication, updaterDownloads);
        m_cnlpParsedFiles.insert(loaderApplication, loaderDownloads);
        m_cnlpParsedFiles.insert(javaApplication, javaDownloads);

        processCnlpDownloadFileList();

        connect(m_updater, SIGNAL(downloadsFinished()),
                SLOT(applicationDownloadFinished()));
        m_updater->setUrlListToDownload(m_filesToDownload);
    } else {
        L_INFO("Unable to read the application cnlp files");
    }
}

void AppUpdater::applicationDownloadFinished()
{
    disconnect(m_updater, SIGNAL(downloadsFinished()),
               this, SLOT(applicationDownloadFinished()));

    L_INFO("Files successfully downloaded");

    // reading hash key only once
    const QString hash_key = HashKey::readHashKey();

    bool downloadsOk = true;

    // check downloaded hash mac
    QMap<Application, QList<QString> >::const_iterator iterator = m_filesToDownload.constBegin();
    while (iterator != m_filesToDownload.constEnd()) {
        const Application application = iterator.key();
        const QList<QString> downloadedFiles = iterator.value();

        // foreach application, checking each file
        foreach(QString downloadedFile, downloadedFiles) {
            if (application == Application::getAppApplication()
                || application == Application::getLoaderApplication()
                || application == Application::getUpdaterApplication()
                || application == Application::getJavaApplication()) {
                QDir dir;

                if (application == Application::getAppApplication()) {
                    dir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::AppDir));
                } else if (application == Application::getLoaderApplication()) {
                    dir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::LoaderDir));
                } else if (application == Application::getUpdaterApplication()) {
                    dir = QDir(m_appPath.getTempDir()).absoluteFilePath(IOConfig::UpdaterDir);
                } else if (application == Application::getJavaApplication()) {
                    dir = QDir(m_appPath.getTempDir()).absoluteFilePath(IOConfig::JavaDir);
                }

                // temporary location of downloaded file
                QString localFile = dir.absoluteFilePath(downloadedFile);

                if (QFile::exists(localFile)) {
                    // if this local temporary file exists
                    // checking its hashmac
                    QString hashmac = HashMac512::hashFromFile(localFile, hash_key);

                    // we find the download corresponding to the downloadedFile to get its expected hashmac
                    bool found = false;
                    foreach(Download download, m_cnlpParsedFiles[application]) {
                        if (download.getHref() == downloadedFile) {
                            found = true;
                            if (hashmac != download.getHashMac()) {
                                L_WARN("Bad hashmac, expected: " + download.getHashMac() + ", found: " + hashmac + " for file: " + localFile);
                                downloadsOk = false;
                            }
                            break;
                        }
                    }
                    if (!found) {
                        // we print a warning if downloaded file was not found in the parsed downloads list
                        // we just log it, in case this happens, but this mustn't
                        L_WARN("Downloaded file not found in the parsed downloads list: " + downloadedFile);
                    }
                } else {
                    // if this file does not exist, we get an error
                    L_ERROR("Downloaded file does not exist " + localFile);
                    downloadsOk = false;
                }
            }
        }

        ++iterator;
    }

    if (downloadsOk) {
        // all downloads are done

        bool buildOk = true;

        // we install only if the download list is not empty or if the remain list is not empty.
        if (doesAppNeedToBeRebuild(Application::getAppApplication())) {
            L_INFO("Need to rebuild and install " + Application::getAppApplication().getName());
            buildOk &= buildApplicationInTempDirectory(Application::getAppApplication());
            L_INFO(Application::getAppApplication().getName() + " build result: " + QString::number(buildOk));
        }
        if (doesAppNeedToBeRebuild(Application::getLoaderApplication())) {
            L_INFO("Need to rebuild and install " + Application::getLoaderApplication().getName());
            buildOk &= buildApplicationInTempDirectory(Application::getLoaderApplication());
            L_INFO(Application::getLoaderApplication().getName() + " build result: " + QString::number(buildOk));
        }
        if (doesAppNeedToBeRebuild(Application::getUpdaterApplication())) {
            L_INFO("Need to rebuild and install " + Application::getUpdaterApplication().getName());
            buildOk &= buildApplicationInTempDirectory(Application::getUpdaterApplication());
            L_INFO(Application::getUpdaterApplication().getName() + " build result: " + QString::number(buildOk));
        }
        if (doesAppNeedToBeRebuild(Application::getJavaApplication())) {
            L_INFO("Need to rebuild and install " + Application::getJavaApplication().getName());
            buildOk &= buildApplicationInTempDirectory(Application::getJavaApplication());
            L_INFO(Application::getJavaApplication().getName() + " build result: " + QString::number(buildOk));
        }

        if (buildOk) {
            // build is ok

            bool appInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getAppApplication())) {
                // if this app needed to be rebuild, we now install it.

                L_INFO("Installing " + Application::getAppApplication().getName());

                const QString appInstallDir = m_appPath.getAppDir().absolutePath();
                const QString appOldDir = m_appPath.getAppDir().absolutePath() + UpdaterConfig::OldDirSuffix;
                const QString appBuildDir = m_appPath.getTempDir().absoluteFilePath(Application::getAppApplication().getName() + UpdaterConfig::BuildDirSuffix);

                // remove an old app directory if it already exists
                if (FileUtils::directoryExists(appOldDir)) {
                    L_INFO("Existing old application directory needs to be removed: " + appOldDir);

                    if (FileUtils::removeDirRecursively(appOldDir)) {
                        L_INFO("Removed " + appOldDir);
                    } else {
                        L_ERROR("Unable to remove " + appOldDir);
                        appInstalledOk = false;
                    }
                }

                if (appInstalledOk) {
                    // we continue only if the old existing directory has been deleted

                    // firstly, we rename the old app directory.
                    if (QDir().rename(appInstallDir, appOldDir)) {
                        L_INFO("Renamed " + appInstallDir + " to " + appOldDir);
                        if (QDir().rename(appBuildDir, appInstallDir)) {
                            L_INFO("Renamed " + appBuildDir + " to " + appInstallDir);
                            if (FileUtils::removeDirRecursively(appOldDir)) {
                                L_INFO("Removed " + appOldDir);
                            } else {
                                L_WARN("Unable to remove " + appOldDir);
                                // not an error
                            }
                        } else {
                            L_ERROR("Unable to rename " + appBuildDir + " to " + appInstallDir);
                            appInstalledOk = false;
                        }
                    } else {
                        L_ERROR("Unable to rename " + appInstallDir + " to " + appOldDir);
                        appInstalledOk = false;
                    }
                }
            } else {
                L_INFO("Application does not need to be updated.");
            }

            if (appInstalledOk) {
                L_INFO("No error reported on application installation.");
            } else {
                L_ERROR("Errors have been reported on application installation.");
            }

            bool loaderInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getLoaderApplication())) {
                // if this app needed to be rebuild, we now install it.

                L_INFO("Installing " + Application::getLoaderApplication().getName());

                const QString loaderInstallDir = m_appPath.getLoaderDir().absolutePath();
                const QString loaderOldDir = m_appPath.getLoaderDir().absolutePath() + UpdaterConfig::OldDirSuffix;
                const QString loaderBuildDir = m_appPath.getTempDir().absoluteFilePath(Application::getLoaderApplication().getName() + UpdaterConfig::BuildDirSuffix);

                // remove an old loader directory if it already exists
                if (FileUtils::directoryExists(loaderOldDir)) {
                    L_INFO("Existing old loader directory needs to be removed: " + loaderOldDir);

                    if (FileUtils::removeDirRecursively(loaderOldDir)) {
                        L_INFO("Removed " + loaderOldDir);
                    } else {
                        L_ERROR("Unable to remove " + loaderOldDir);
                        loaderInstalledOk = false;
                    }
                }

                if (loaderInstalledOk) {
                    // we continue only if the old existing directory has been deleted

                    // firstly, we rename the old app directory.
                    if (QDir().rename(loaderInstallDir, loaderOldDir)) {
                        L_INFO("Renamed " + loaderInstallDir + " to " + loaderOldDir);
                        if (QDir().rename(loaderBuildDir, loaderInstallDir)) {
                            L_INFO("Renamed " + loaderBuildDir + " to " + loaderInstallDir);
                            if (FileUtils::removeDirRecursively(loaderOldDir)) {
                                L_INFO("Removed " + loaderOldDir);
                            } else {
                                L_WARN("Unable to remove " + loaderOldDir);
                                // not an error
                            }
                        } else {
                            L_ERROR("Unable to rename " + loaderBuildDir + " to " + loaderInstallDir);
                            loaderInstalledOk = false;
                        }
                    } else {
                        L_ERROR("Unable to rename " + loaderInstallDir + " to " + loaderOldDir);
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
            } else {
                L_INFO("Loader does not need to be updated.");
            }

            if (loaderInstalledOk) {
                L_INFO("No error reported on loader installation.");
            } else {
                L_ERROR("Errors have been reported on loader installation.");
            }

            bool updaterInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getUpdaterApplication())) {
                // if this app needed to be rebuild, we now install it.

                // we can not overwrite ourselves
                if (m_localUpdaterVersion != m_remoteUpdaterVersion) {
                    L_INFO("Installing " + Application::getUpdaterApplication().getName());

                    const QString updaterInstallDir = m_appPath.getUpdaterDir().absoluteFilePath(m_remoteUpdaterVersion);
                    const QString updaterBuildDir = m_appPath.getTempDir().absoluteFilePath(Application::getUpdaterApplication().getName() + UpdaterConfig::BuildDirSuffix);

                    if (FileUtils::directoryExists(updaterInstallDir)) {
                        // it should not exist with the previous condition, but in case it exists
                        // (it is not used because local and remote versions differ), we need to remove it
                        if (FileUtils::removeDirRecursively(updaterInstallDir)) {
                            L_INFO("Removed " + updaterInstallDir);
                        } else {
                            L_ERROR("Unable to remove " + updaterInstallDir);
                            updaterInstalledOk = false;
                        }
                    }

                    if (updaterInstalledOk) {
                        // we continue only if there is no error

                        // local and remote versions differ
                        if (QDir().rename(updaterBuildDir, updaterInstallDir)) {
                            L_INFO("Renamed " + updaterBuildDir + " to " + updaterInstallDir);
                            // nothing to remove. deletion will be done at next launch.
                            // extract app from dmg (macos).
                            if (m_appPath.prepareUpdater(m_remoteUpdaterVersion)) {
                                L_INFO("Updater " + m_remoteUpdaterVersion + " prepared.");
                                // write new updater version number
                                Settings * settings = Settings::getInstance();
                                settings->setUpdaterVersion(m_remoteUpdaterVersion);
                                if (settings->writeSettings()) {
                                    L_INFO("Written version " + m_remoteUpdaterVersion + " to settings.");
                                } else {
                                    L_ERROR("Unable to write version " + m_remoteUpdaterVersion + " to settings.");
                                    updaterInstalledOk = false;
                                }
                            } else {
                                L_ERROR("Unable to prepare Updater " + m_remoteUpdaterVersion + ".");
                                updaterInstalledOk = false;
                            }
                        } else {
                            L_ERROR("Unable to rename " + updaterBuildDir + " to " + updaterInstallDir);
                            updaterInstalledOk = false;
                        }
                    }
                } else {
                    L_WARN("Remote and local updaters differ but have same version number, no possible update.");
                }
            } else {
                L_INFO("Updater does not need to be updated.");
            }

            if (updaterInstalledOk) {
                L_INFO("No error reported on updater installation.");
            } else {
                L_ERROR("Errors have been reported on updater installation.");
            }

            bool javaInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getJavaApplication())) {
                // if this app needed to be rebuild, we now install it.

                // we can overwrite java because it must not be used by us.
                L_INFO("Installing " + Application::getJavaApplication().getName());

                const QString javaInstallDir = m_appPath.getJavaDir().absoluteFilePath(m_remoteJavaVersion);
                const QString javaBuildDir = m_appPath.getTempDir().absoluteFilePath(Application::getJavaApplication().getName() + UpdaterConfig::BuildDirSuffix);

                if (FileUtils::directoryExists(javaInstallDir)) {
                    // remove existing java dir if it exists
                    if (FileUtils::removeDirRecursively(javaInstallDir)) {
                        L_INFO("Removed " + javaInstallDir);
                    } else {
                        L_ERROR("Unable to remove " + javaInstallDir);
                        javaInstalledOk = false;
                    }
                }

                if (javaInstalledOk) {
                    // we continue only if there is no error

                    // local and remote versions differ
                    if (QDir().rename(javaBuildDir, javaInstallDir)) {
                        L_INFO("Renamed " + javaBuildDir + " to " + javaInstallDir);
                        // nothing to remove. deletion will be done at next launch.
                        // extract zip.
                        if (prepareJava(m_remoteJavaVersion, true)) {
                            L_INFO("Java " + m_remoteJavaVersion + " force prepared.");
                            // write new java version number
                            Settings * settings = Settings::getInstance();
                            settings->setJavaVersion(m_remoteJavaVersion);
                            if (settings->writeSettings()) {
                                L_INFO("Written version " + m_remoteJavaVersion + " to settings.");
                            } else {
                                L_ERROR("Unable to write version " + m_remoteJavaVersion + " to settings.");
                                javaInstalledOk = false;
                            }
                        } else {
                            L_ERROR("Unable to force prepare Java " + m_remoteJavaVersion + ".");
                            javaInstalledOk = false;
                        }
                    } else {
                        L_ERROR("Unable to rename " + javaBuildDir + " to " + javaInstallDir);
                        javaInstalledOk = false;
                    }
                }
            } else {
                L_INFO("Java does not need to be updated.");

                if (javaInstalledOk) {
                    const QString javaDistDir = m_appPath.getJavaDir().absoluteFilePath(m_remoteJavaVersion + QDir::separator() + IOConfig::JavaSubDirName);

                    if (!FileUtils::directoryExists(javaDistDir)) {
                        L_INFO("Java dist directory does not exist. " + javaDistDir);
                        if (prepareJava(m_remoteJavaVersion, false)) {
                            L_INFO("Java " + m_remoteJavaVersion + " soft prepared.");
                        } else {
                            L_INFO("Unable to soft prepare Java " + m_remoteJavaVersion + ".");
                            javaInstalledOk = false;
                        }
                    }
                }
            }

            if (javaInstalledOk) {
                L_INFO("No error reported on java installation.");
            } else {
                L_ERROR("Errors have been reported on java installation.");
            }

            if (appInstalledOk && loaderInstalledOk && updaterInstalledOk && javaInstalledOk) {
                // application, loader and updater didn't throw error, updating cnlp files.

                L_INFO("Installing Cnlp files.");

                const QString cnlpInstallDir = m_appPath.getCnlpDir().absolutePath();
                const QString cnlpOldDir = m_appPath.getCnlpDir().absolutePath() + UpdaterConfig::OldDirSuffix;
                const QString cnlpBuildDir = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpName);

                bool cnlpInstalledOk = true;

                // remove an old cnlp directory if it already exists
                if (FileUtils::directoryExists(cnlpOldDir)) {
                    L_INFO("Existing old cnlp directory needs to be removed: " + cnlpOldDir);

                    if (FileUtils::removeDirRecursively(cnlpOldDir)) {
                        L_INFO("Removed " + cnlpOldDir);
                    } else {
                        L_ERROR("Unable to remove " + cnlpOldDir);
                        cnlpInstalledOk = false;
                    }
                }

                if (cnlpInstalledOk) {
                    // we continue only if the old existing directory has been deleted

                    // firstly, we rename the old cnlp directory.
                    if (QDir().rename(cnlpInstallDir, cnlpOldDir)) {
                        L_INFO("Renamed " + cnlpInstallDir + " to " + cnlpOldDir);
                        if (QDir().rename(cnlpBuildDir, cnlpInstallDir)) {
                            L_INFO("Renamed " + cnlpBuildDir + " to " + cnlpInstallDir);
                            if (FileUtils::removeDirRecursively(cnlpOldDir)) {
                                L_INFO("Removed " + cnlpOldDir);
                            } else {
                                L_WARN("Unable to remove " + cnlpOldDir);
                                // not an error
                            }
                        } else {
                            L_ERROR("Unable to rename " + cnlpBuildDir + " to " + cnlpInstallDir);
                            cnlpInstalledOk = false;
                        }
                    } else {
                        L_ERROR("Unable to rename " + cnlpInstallDir + " to " + cnlpOldDir);
                        cnlpInstalledOk = false;
                    }
                }

                if (cnlpInstalledOk) {
                    L_INFO("No error reported on cnlp installation.");

                    L_INFO("START THE APPLICATION");
                } else {
                    L_ERROR("Errors have been reported on cnlp installation.");
                }
            }
        } else {
            L_INFO("BUILD KO !!!!");
        }
    } else {
    }
}

bool AppUpdater::buildApplicationInTempDirectory(const Application &_application)
{
    bool buildOk = true;
    QDir tempAppDir;
    QDir appDir;
    QString appBuild;

    if (_application == Application::getAppApplication() ||
        _application == Application::getLoaderApplication() ||
        _application == Application::getUpdaterApplication() ||
        _application == Application::getJavaApplication()) {
        if (_application == Application::getAppApplication()) {
            tempAppDir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::AppDir));
            appDir = QDir(m_appPath.getAppDir());
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + UpdaterConfig::BuildDirSuffix);
        } else if (_application == Application::getLoaderApplication()) {
            tempAppDir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::LoaderDir));
            appDir = QDir(m_appPath.getLoaderDir());
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + UpdaterConfig::BuildDirSuffix);
        } else if (_application == Application::getUpdaterApplication()) {
            tempAppDir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::UpdaterDir));
            appDir = QDir(m_appPath.getUpdaterDir().absoluteFilePath(m_localUpdaterVersion));
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + UpdaterConfig::BuildDirSuffix);
        } else if (_application == Application::getJavaApplication()) {
            tempAppDir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::JavaDir));
            appDir = QDir(m_appPath.getJavaDir().absoluteFilePath(m_localJavaVersion));
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + UpdaterConfig::BuildDirSuffix);
        }
    } else {
        // nothing to do, return true
        return true;
    }

    // remove temp/{application}-build directory
    L_INFO("Removing previous build directory: " + appBuild);
    FileUtils::removeDirRecursively(appBuild);

    if (QDir().mkpath(appBuild)) {
        L_INFO("Creating empty build directory: " + appBuild);

        const QList<QString> downloadedFiles = m_filesToDownload[_application];

        foreach(QString downloadedFile, downloadedFiles) {
            QString localSourceFile = tempAppDir.absoluteFilePath(downloadedFile);
            QString destinationFile = appBuild + QDir::separator() + downloadedFile;

            if (QFile::exists(localSourceFile)) {
                L_INFO("download source: " + localSourceFile);
                L_INFO("destination:     " + destinationFile);
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

        const QList<QString> keepFiles = m_filesToKeep[_application];

        foreach(QString keepFile, keepFiles) {
            QString localSourceFile = appDir.absoluteFilePath(keepFile);
            QString destinationFile = appBuild + QDir::separator() + keepFile;

            if (QFile::exists(localSourceFile)) {
                L_INFO("keep source: " + localSourceFile);
                L_INFO("destination: " + destinationFile);
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
            dir = QDir(appPath.getUpdaterDir().absoluteFilePath(m_localUpdaterVersion));
        } else if (_application == Application::getJavaApplication()) {
            dir = QDir(appPath.getJavaDir().absoluteFilePath(m_localJavaVersion));
        }

        QDirIterator it(dir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filename = dir.relativeFilePath(it.next());

#ifdef Q_OS_MACOS
                // to disable false application rebuilding,
                // on macos, we ignore updater/<version>/updater.app/
                if (_application == Application::getUpdaterApplication()
                    && filename.startsWith(IOConfig::UpdaterName + IOConfig::MacOsAppExtension + QDir::separator())) {
                    L_INFO("Ignoring file: " + filename);
                    continue;
                }
                // and loader/loader.app/
                if (_application == Application::getLoaderApplication()
                    && filename.startsWith(IOConfig::LoaderName + IOConfig::MacOsAppExtension + QDir::separator())) {
                    L_INFO("Ignoring file: " + filename);
                    continue;
                }

                // and .DS_Store on macos
                if (filename.toLower().endsWith(".ds_store")) {
                    L_INFO("Ignoring file: " + filename);
                    continue;
                }
#endif

#ifdef Q_OS_WIN
                // and Thumbs.db on windows
                if (filename.toLower().endsWith("thumbs.db")) {
                    L_INFO("Ignoring file: " + filename);
                    continue;
                }
#endif

            // and java/<java-version>/dist/
            if (_application == Application::getJavaApplication()
                && filename.startsWith(IOConfig::JavaSubDirName + QDir::separator())) {
                L_INFO("Ignoring file: " + filename);
                continue;
            }

            fileList.append(filename);
        }
    }

    return fileList;
}

QMap<Application, QList<QString> > AppUpdater::getLocalFiles()
{
    QMap<Application, QList<QString> > fileList;

    fileList.insert(Application::getLoaderApplication(), getLocalFiles(Application::getLoaderApplication()));
    fileList.insert(Application::getUpdaterApplication(), getLocalFiles(Application::getUpdaterApplication()));
    fileList.insert(Application::getAppApplication(), getLocalFiles(Application::getAppApplication()));
    fileList.insert(Application::getJavaApplication(), getLocalFiles(Application::getJavaApplication()));

    return fileList;
}

void AppUpdater::processCnlpDownloadFileList()
{
    // empty the lists
    m_filesToDownload.clear();
    m_filesToKeep.clear();
    m_remainingFiles.clear();

    // reading hash key only once
    const QString hash_key = HashKey::readHashKey();

    // list of all local files (installed and in temporary directory)
    const QMap<Application, QList<QString> > allLocalFiles = AppUpdater::getLocalFiles();

    // iterate over files read from cnlp files
    QMap<Application, QList<Download> >::const_iterator iterator = m_cnlpParsedFiles.constBegin();

    while (iterator != m_cnlpParsedFiles.constEnd()) {
        const Application application = iterator.key();

        L_INFO("Processing Application: " + application.getName());

        // lists init
        m_filesToDownload.insert(application, QList<QString>());
        m_filesToKeep.insert(application, QList<QString>());

        const QList<Download> parsedDownloads = iterator.value();

        // local files of the current application, installed only
        QList<QString> localFilesOfCurrentApplication = allLocalFiles[application];

        foreach(Download parsedDownload, parsedDownloads) {
            L_INFO("Remote file: " + parsedDownload.getHref() + " has hash: " + parsedDownload.getHashMac());

            if (localFilesOfCurrentApplication.contains(parsedDownload.getHref())) {
                QString localFile = "";
                if (application == Application::getAppApplication()) {
                    localFile = m_appPath.getAppDir().absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getLoaderApplication()) {
                    localFile = m_appPath.getLoaderDir().absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getUpdaterApplication()) {
                    localFile = m_appPath.getUpdaterDir().absoluteFilePath(m_localUpdaterVersion + QDir::separator() + parsedDownload.getHref());
                } else if (application == Application::getJavaApplication()) {
                    localFile = m_appPath.getJavaDir().absoluteFilePath(m_localJavaVersion + QDir::separator() + parsedDownload.getHref());
                }

                // must be true but we test anyway
                if (QFile::exists(localFile)) {
                    QString hashmac = HashMac512::hashFromFile(localFile, hash_key);
                    L_INFO("Local file: " + localFile + " has hashmac: " + hashmac);

                    if (hashmac == parsedDownload.getHashMac()) {
                        L_INFO("Local and remote files are identical. Add to keep list.");
                        // add file to download list because file is out of date
                        m_filesToKeep[application].append(parsedDownload.getHref());
                        // remove from the local list
                        localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                    } else {
                        L_INFO("Local and remote files differ. Add to download list.");
                        // add file to download list because file is out of date
                        m_filesToDownload[application].append(parsedDownload.getHref());
                        // remove from the local list
                        localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                    }
                } else {
                    L_INFO("Local file does not exist. Add to download list.");
                    // add file to download list because it doesn't exist
                    m_filesToDownload[application].append(parsedDownload.getHref());
                    // remove from the local list
                    localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                }
            } else {
                L_INFO("Local file does not exist. Add to download list.");
                // add file to download list because it doesn't exist
                m_filesToDownload[application].append(parsedDownload.getHref());
                // remove from the local list
                localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
            }
        }

        // remaining files, those who no longer need to be in the app
        m_remainingFiles[application] = localFilesOfCurrentApplication;

        ++iterator;
    }
}

bool AppUpdater::doesAppNeedToBeRebuild(const Application &_application)
{
    if (m_filesToDownload.contains(_application)) {
        return !m_filesToDownload[_application].isEmpty()
               || !m_remainingFiles[_application].isEmpty();
    }

    return false;
}

bool AppUpdater::prepareJava(const QString &_version, bool _forceOverwrite)
{
    QDir javaVersionDir = m_appPath.getJavaDir().absoluteFilePath(_version);

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
                        connect(&zip, SIGNAL(finished()), &loop, SLOT(quit()));
                        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
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
