#include "downloadmanager.h"

#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QSaveFile>
#include <stdio.h>
#include "config/global.h"

DownloadManager::DownloadManager(QObject *_parent) : QObject(_parent),
	m_currentDownloaderCount(0),
	m_currentFileCount(0),
	m_saveFile(0)
{
}

DownloadManager::~DownloadManager() {
	delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QStringList &urlList)
{
	m_currentFileCount = 0;
	m_currentDownloaderCount = 0;

	// TODO signal pour initialiser la progress bar ?
	// TODO détail du download ?
    foreach (QString url, urlList) {
		if (m_downloadQueue.isEmpty())
			QTimer::singleShot(0, this, SLOT(startNextDownload()));

		m_downloadQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
		++m_currentFileCount;
	}

    if (m_downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

QString DownloadManager::getFilename(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

	// TODO https://stackoverflow.com/a/1395173/8570451
	// se baser sur inline / attachment (de préférence) pour le nom du fichier.
    if (basename.isEmpty())
        basename = "download";

	// on écrase systématiquement

    return basename;
}

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty()) {
        printf("%d/%d files downloaded successfully\n", m_currentDownloaderCount, m_currentFileCount);
        emit finished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();
    m_currentFilename = getFilename(url);

	m_saveFile = new QSaveFile(m_currentFilename);

	qDebug() << "temporary file:" << m_saveFile->fileName();
	if (!m_saveFile->open(QIODevice::WriteOnly)) {
		qDebug() << "Error opening temporary file for URL: " << url.toEncoded().constData();
        startNextDownload();
        return;                 // skip this download
	}

    QNetworkRequest request(url);
	request.setRawHeader("User-Agent", "Covalia-Downloader");
    currentDownload = m_manager.get(request);

    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));
	connect(currentDownload, SIGNAL(error(QNetworkReply::NetworkError)),
			SLOT(errorOccured(QNetworkReply::NetworkError)));
	// TODO redirect


    qDebug() << "Downloading" << url.toEncoded().constData();
    m_downloadTime.start();
}

void DownloadManager::updateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	emit downloadProgress(bytesReceived, bytesTotal);

    // calculate the download speed
    double speed = bytesReceived * 10000 / m_downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "bytes/sec";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "kB/s";
    } else {
        speed /= 1024*1024;
        unit = "MB/s";
    }

	emit downloadSpeedMessage(QString::fromLatin1("%1 %2")
                            .arg(speed, 3, 'f', 1).arg(unit));
}

void DownloadManager::downloadFinished()
{
    // TODO progressBar.clear();
	m_saveFile->commit();

	delete m_saveFile;
	m_saveFile = 0;

    if (currentDownload->error()) {
        // download failed
        fprintf(stderr, "Failed: %s\n", qPrintable(currentDownload->errorString()));
    } else {
        printf("Succeeded.\n");
        ++m_currentDownloaderCount;
    }

    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    m_saveFile->write(currentDownload->readAll());
}

void DownloadManager::errorOccured(QNetworkReply::NetworkError _error) {
	qDebug() << _error;
	m_saveFile->cancelWriting();
}

