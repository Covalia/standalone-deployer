#include "downloader/appdownloader.h"
#include "network/downloadmanager.h"
#include "config/global.h"
#include <QDebug>

AppDownloader::AppDownloader(const QUrl &_appUrl, const QDir &_appInstallDir, QObject *_parent) : QObject(_parent),
	m_downloader(0),
	m_appTreeManager(0)
{
	m_appTreeManager = new AppTreeManager(_appInstallDir, this);
    m_appUrl = _appUrl;

    QNetworkProxy proxy;

    // TODO récupérer ici la configuration du proxy.

    m_downloader = new DownloadManager(m_appTreeManager->getTempDirPath(), _appUrl, proxy, this);

    connect(m_downloader, SIGNAL(downloadProgress(qint64, qint64)),
        SLOT(updateProgress(qint64, qint64)));
    connect(m_downloader, SIGNAL(downloadSpeedMessage(const QString &)),
        SLOT(updateDownloadSpeedMessage(const QString &)));
    connect(m_downloader, SIGNAL(remainingTimeMessage(const QString &)),
        SLOT(updateRemainingTimeMessage(const QString &)));
    connect(m_downloader, SIGNAL(downloadFileMessage(const QString &)),
        SLOT(updateDownloadFileMessage(const QString &)));
    connect(m_downloader, SIGNAL(totalDownloadProgress(qint64, qint64)),
        SLOT(updateTotalDownloadProgress(qint64, qint64)));
    connect(m_downloader, SIGNAL(downloadsFinished()),
            SLOT(downloadFinished()));
}

AppDownloader::~AppDownloader()
{
	delete m_downloader;
	delete m_appTreeManager;
}

void AppDownloader::start()
{
	qDebug() << "start app downloader";
	emit serverUrlMessage(m_appUrl);

	bool result = m_appTreeManager->makeAppDirectories();
	qDebug() << "makeAppDirectories" << result;

    QList<QUrl> urls;

    urls << QUrl("lib/covotem-macosx-native.jar");
    urls << QUrl("lib/covotemLoader.jar");
    urls << QUrl("lib/covotem.jar");
    urls << QUrl("lib/external/annotations.jar");
    urls << QUrl("lib/external/balloontip-1.2.4.1.jar");


    // urls << m_appUrl + Global::AppCnlpRemoteFilename;

    // TODO transmettre le nom de l'application avec les url des fichiers, il s'agit du sous dossier
    m_downloader->setUrlListToDownload("Application", urls);
}

void AppDownloader::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
	emit downloadProgress(_bytesReceived, _bytesTotal);
}

void AppDownloader::updateDownloadSpeedMessage(const QString &_speed)
{
    emit downloadSpeedMessage(_speed);
}

void AppDownloader::updateRemainingTimeMessage(const QString &_time)
{
    emit remainingTimeMessage(_time);
}

void AppDownloader::updateDownloadFileMessage(const QString &_file) {
    emit downloadFileMessage(_file);
}

void AppDownloader::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal) {
    emit totalDownloadProgress(_bytesReceived, _bytesTotal);
}

void AppDownloader::downloadFinished() {
    qDebug() << "TODO read the cnlp";
    // TODO read xml.

}
