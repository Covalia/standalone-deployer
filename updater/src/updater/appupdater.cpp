#include "updater/appupdater.h"
#include "network/downloadmanager.h"
#include "updater/config.h"
#include "io/config.h"
#include "xml/deploymentxml.h"
#include "updater/hash_key.h"
#include "utils/hashmac/hashmac512.h"
#include "io/fileutils.h"
#include "settings/settings.h"

#include <QDirIterator>

AppUpdater::AppUpdater(const QUrl &_appUrl, const QDir &_appInstallDir, QObject * _parent) : QObject(_parent),
    m_updater(0),
    m_appPath(Utils::getAppPath()),
    m_remoteUpdaterVersion(""),
    m_localUpdaterVersion("")
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

    QString applicationCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::AppCnlpLocalFilename);
    QString updaterCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::UpdaterCnlpLocalFilename);
    QString loaderCnlpPath = m_appPath.getTempDir().absoluteFilePath(IOConfig::CnlpDir + QString("/") + UpdaterConfig::LoaderCnlpLocalFilename);
    L_INFO("Files to read: " + applicationCnlpPath);
    L_INFO("Files to read: " + updaterCnlpPath);
    L_INFO("Files to read: " + loaderCnlpPath);

    DeploymentXML applicationXml(applicationCnlpPath);
    DeploymentXML updaterXml(updaterCnlpPath);
    DeploymentXML loaderXml(loaderCnlpPath);

    if (applicationXml.read() && updaterXml.read() && loaderXml.read()) {
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

        const Application appApplication = Application::getAppApplication();
        const Application updaterApplication = Application::getUpdaterApplication();
        const Application loaderApplication = Application::getLoaderApplication();

        // TODO refactor into an helper class?
        QString osValue;
#ifdef Q_OS_MACOS
            osValue = DeploymentXML::OsMacOsValue;
#endif

#ifdef Q_OS_WIN
            osValue = DeploymentXML::OsWindowsValue;
#endif
        QList<Download> appDownloads = applicationXml.getDownloads(appApplication, osValue);
        QList<Download> updaterDownloads = updaterXml.getDownloads(updaterApplication, osValue);
        QList<Download> loaderDownloads = loaderXml.getDownloads(loaderApplication, osValue);

        m_cnlpParsedFiles.clear();
        m_cnlpParsedFiles.insert(appApplication, appDownloads);
        m_cnlpParsedFiles.insert(updaterApplication, updaterDownloads);
        m_cnlpParsedFiles.insert(loaderApplication, loaderDownloads);

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

        foreach(QString downloadedFile, downloadedFiles) {
            if (application == Application::getAppApplication()
                || application == Application::getLoaderApplication()
                || application == Application::getUpdaterApplication()) {
                QDir dir;

                if (application == Application::getAppApplication()) {
                    dir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::AppDir));
                } else if (application == Application::getLoaderApplication()) {
                    dir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::LoaderDir));
                } else if (application == Application::getUpdaterApplication()) {
                    dir = QDir(m_appPath.getTempDir()).absoluteFilePath(IOConfig::UpdaterDir);
                }

                QString localFile = dir.absoluteFilePath(downloadedFile);

                if (QFile::exists(localFile)) {
                    QString hashmac = HashMac512::hashFromFile(localFile, hash_key);

                    foreach(Download download, m_cnlpParsedFiles[application]) {
                        if (download.getHref() == downloadedFile) {
                            if (hashmac != download.getHashMac()) {
                                L_INFO("Bad hashmac, expected: " + download.getHashMac() + ", found: " + hashmac + " for file: " + localFile);
                                downloadsOk = false;
                            }
                            break;
                        }
                    }
                } else {
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

        if (buildOk) {
            // build is ok

            bool appInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getAppApplication())) {
                // if this app needed to be rebuild, we now install it.

                L_INFO("Installing " + Application::getAppApplication().getName());

                const QString appInstallDir = m_appPath.getAppDir().absolutePath();
                const QString appOldDir = m_appPath.getAppDir().absolutePath() + UpdaterConfig::OldDirSuffix;
                const QString appBuildDir = m_appPath.getTempDir().absoluteFilePath(Application::getAppApplication().getName() + UpdaterConfig::BuildDirSuffix);

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

            if (appInstalledOk) {
                L_INFO("NO ERROR ON APPLICATION INSTALLATION");
            }

            bool loaderInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getLoaderApplication())) {
                // if this app needed to be rebuild, we now install it.

                L_INFO("Installing " + Application::getLoaderApplication().getName());

                const QString loaderInstallDir = m_appPath.getLoaderDir().absolutePath();
                const QString loaderOldDir = m_appPath.getLoaderDir().absolutePath() + UpdaterConfig::OldDirSuffix;
                const QString loaderBuildDir = m_appPath.getTempDir().absoluteFilePath(Application::getLoaderApplication().getName() + UpdaterConfig::BuildDirSuffix);

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

            if (loaderInstalledOk) {
                L_INFO("NO ERROR ON LOADER INSTALLATION");
            }

            bool updaterInstalledOk = true;
            if (doesAppNeedToBeRebuild(Application::getUpdaterApplication())) {
                // if this app needed to be rebuild, we now install it.
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
                                }
                                else {
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
            }

            if (updaterInstalledOk) {
                L_INFO("NO ERROR ON UPDATER INSTALLATION");
            }

            if (appInstalledOk && loaderInstalledOk && updaterInstalledOk) {
                L_INFO("COPY CNLP FILES");
            }

            L_INFO("START THE APPLICATION");
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
        _application == Application::getUpdaterApplication()) {
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
        || _application == Application::getUpdaterApplication()) {
        QDir dir;

        if (_application == Application::getAppApplication()) {
            dir = QDir(appPath.getAppDir());
        } else if (_application == Application::getLoaderApplication()) {
            dir = QDir(appPath.getLoaderDir());
        } else if (_application == Application::getUpdaterApplication()) {
            dir = QDir(appPath.getUpdaterDir().absoluteFilePath(m_localUpdaterVersion));
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
#endif

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
