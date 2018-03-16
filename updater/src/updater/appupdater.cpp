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

        m_remoteUpdaterVersion = updaterXml.getVersion();
        Settings *settings = Settings::getInstance();
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
                    L_INFO(hashmac);

                    foreach(Download download, m_cnlpParsedFiles[application]) {
                        if (download.getHref() == downloadedFile) {
                            if (hashmac == download.getHashMac()) {
                            } else {
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

        buildOk &= buildApplicationInTempDirectory(Application::getAppApplication());
        L_INFO("Application build result: " + QString::number(buildOk));
        buildOk &= buildApplicationInTempDirectory(Application::getLoaderApplication());
        L_INFO("Loader build result: " + QString::number(buildOk));
        buildOk &= buildApplicationInTempDirectory(Application::getUpdaterApplication());
        L_INFO("Updater build result: " + QString::number(buildOk));

        if (buildOk) {

            // if (!m_filesToDownload[Application::getAppApplication()].isEmpty() && m_filesToKeep[Application::getAppApplication()].isEmpty()) {

            L_INFO("Installing " + Application::getAppApplication().getName());
            if (QDir().rename(m_appPath.getAppDir().absolutePath(), m_appPath.getAppDir().absolutePath() + "-old")) {
                if (QDir().rename(m_appPath.getTempDir().absoluteFilePath(Application::getAppApplication().getName() + "-build"), m_appPath.getAppDir().absolutePath())) {
                    FileUtils::removeDirRecursively(m_appPath.getAppDir().absolutePath() + "-old");
                }
            }

            L_INFO("Installing " + Application::getLoaderApplication().getName());
            if (QDir().rename(m_appPath.getLoaderDir().absolutePath(), m_appPath.getLoaderDir().absolutePath() + "-old")) {
                if (QDir().rename(m_appPath.getTempDir().absoluteFilePath(Application::getLoaderApplication().getName() + "-build"), m_appPath.getLoaderDir().absolutePath())) {
                    FileUtils::removeDirRecursively(m_appPath.getLoaderDir().absolutePath() + "-old");
                }
            }
            // extract app from dmg (macos).
            m_appPath.prepareLoader();

            if (m_localUpdaterVersion != m_remoteUpdaterVersion) {
                L_INFO("Installing " + Application::getUpdaterApplication().getName());
                // les versions locales et distantes doivent être différentes
                if (QDir().rename(m_appPath.getTempDir().absoluteFilePath(Application::getUpdaterApplication().getName() + "-build" + QDir::separator() + m_remoteUpdaterVersion),
                             m_appPath.getUpdaterDir().absoluteFilePath(m_remoteUpdaterVersion))) {
                }
                // extract app from dmg (macos).
                m_appPath.prepareUpdater(m_remoteUpdaterVersion);
                // write new updater version number
                Settings *settings = Settings::getInstance();
                settings->setUpdaterVersion(m_remoteUpdaterVersion);
                settings->writeSettings();
            } else {
                L_INFO("Updater local and remote versions are identical, no update.");
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
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + "-build");
        } else if (_application == Application::getLoaderApplication()) {
            tempAppDir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::LoaderDir));
            appDir = QDir(m_appPath.getLoaderDir());
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + "-build");
        } else if (_application == Application::getUpdaterApplication()) {
            tempAppDir = QDir(m_appPath.getTempDir().absoluteFilePath(IOConfig::UpdaterDir));
            appDir = QDir(m_appPath.getUpdaterDir().absoluteFilePath(m_remoteUpdaterVersion));
            appBuild = m_appPath.getTempDir().absoluteFilePath(_application.getName() + "-build" + QDir::separator() + m_remoteUpdaterVersion);
        }
    } else {
        return buildOk;
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
            dir = QDir(appPath.getUpdaterDir());
        }

        QDirIterator it(dir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filename = dir.relativeFilePath(it.next());
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
                    localFile = m_appPath.getUpdaterDir().absoluteFilePath(parsedDownload.getHref());
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

        ++iterator;
    }

}
