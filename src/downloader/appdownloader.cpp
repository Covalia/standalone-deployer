#include "appdownloader.h"
#include "network/downloadmanager.h"
#include "config/global.h"
#include <QDebug>

AppDownloader::AppDownloader(QString _appUrl, QString _appInstallDir, QObject *_parent) : QObject(_parent),
	m_downloader(0),
	m_appTreeManager(0)
{
	m_appTreeManager = new AppTreeManager(_appInstallDir, this);
    m_downloader = new DownloadManager(m_appTreeManager->getTempDirPath(), this);
	m_appUrl = _appUrl;

}

AppDownloader::~AppDownloader()
{
	delete m_downloader;
	delete m_appTreeManager;
}

void AppDownloader::start()
{
	qDebug() << "start app downloader";

	connect(m_downloader, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
	connect(m_downloader, SIGNAL(downloadSpeedMessage(QString)), this, SLOT(updateDownloadSpeedMessage(QString)));


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

void AppDownloader::updateDownloadSpeedMessage(QString _message)
{
	emit downloadSpeedMessage(_message);
}

