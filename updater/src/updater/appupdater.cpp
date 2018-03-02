#include "updater/appupdater.h"
#include "network/downloadmanager.h"
#include "updater/config.h"
#include "io/config.h"
#include "xml/deploymentxml.h"

AppUpdater::AppUpdater(const QUrl &_appUrl, const QDir &_appInstallDir, QObject * _parent) : QObject(_parent),
    m_updater(0),
    m_appPath(Utils::getAppPath())
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
        connect(m_updater, SIGNAL(downloadsFinished()),
                SLOT(applicationDownloadFinished()));

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

        QMap<Application, QList<Download> > downloads;
        downloads.insert(appApplication, appDownloads);
        downloads.insert(updaterApplication, updaterDownloads);
        downloads.insert(loaderApplication, loaderDownloads);

        QMap<Application, QList<QUrl> > urls;

        // TODO prévoir ici la comparaison des hashs
        urls = downloadsToUrls(downloads);

        m_updater->setUrlListToDownload(urls);
    } else {
        L_INFO("Unable to read the application cnlp files");
    }
}

void AppUpdater::applicationDownloadFinished()
{
    disconnect(m_updater, SIGNAL(downloadsFinished()),
               this, SLOT(applicationDownloadFinished()));

    L_INFO("Files successfully downloaded");
}

QMap<Application, QList<QUrl> > AppUpdater::downloadsToUrls(const QMap<Application, QList<Download> > &_downloadsMap)
{
    AppPath appPath = Utils::getAppPath();

    QMap<Application, QList<QUrl> > map;

    QMap<Application, QList<Download> >::const_iterator iterator = _downloadsMap.constBegin();
    while (iterator != _downloadsMap.constEnd()) {

        const Application application = iterator.key();
        const QList<Download> downloads = iterator.value();

        L_INFO("Processing Application: " + application.getName());
        QList<QUrl> urls;

        foreach(Download download, downloads) {

            L_INFO("Remote file: " + download.getHref() + " has hash: " + download.getHashMac());

            QString localFile = "";
            if (application == Application::getAppApplication()) {
                localFile = appPath.getAppDir().absoluteFilePath(download.getHref());
            } else if (application == Application::getLoaderApplication()) {
                localFile = appPath.getLoaderDir().absoluteFilePath(download.getHref());
            } else if (application == Application::getUpdaterApplication()) {
                localFile = appPath.getUpdaterDir().absoluteFilePath(download.getHref());
            }

            L_INFO("Local file: " + localFile);

            urls.append(QUrl(download.getHref()));
        }

        if (!urls.isEmpty()) {
            map.insert(application, urls);
        }

        ++iterator;
    }

    return map;
}

