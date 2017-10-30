#include "appdownloader.h"
#include "network/downloadmanager.h"
#include "config/global.h"
#include <QDebug>

AppDownloader::AppDownloader(const QString &_appUrl, const QString &_appInstallDir, QObject *_parent) : QObject(_parent),
	m_downloader(0),
	m_appTreeManager(0)
{
	m_appTreeManager = new AppTreeManager(_appInstallDir, this);
    m_downloader = new DownloadManager(m_appTreeManager->getTempDirPath(), this);
	m_appUrl = _appUrl;

    connect(m_downloader, SIGNAL(downloadProgress(qint64, qint64)),
        SLOT(updateProgress(qint64, qint64)));
    connect(m_downloader, SIGNAL(downloadSpeedMessage(const QString &)),
        SLOT(updateDownloadSpeedMessage(const QString &)));
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

	// QStringList urls;
	// urls << "http://dev.covalia.fr/lanceur_covotem.php" <<
	// 	"http://ftp.nl.debian.org/debian/dists/stretch/main/installer-amd64/current/images/netboot/mini.iso";
	QStringList urls;
	urls << m_appUrl + Global::AppCnlpRelativePath;

    m_downloader->setUrlListToDownload(urls);
}

void AppDownloader::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
	emit downloadProgress(_bytesReceived, _bytesTotal);
}

void AppDownloader::updateDownloadSpeedMessage(const QString &_speed)
{
    emit downloadSpeedMessage(_speed);
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
