#include "downloadmanager.h"
#include <QDebug>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QSaveFile>
#include <stdio.h>
#include "config/global.h"

DownloadManager::DownloadManager(const QString &_saveDir, QObject *_parent) : QObject(_parent),
	m_currentDownloaderCount(0),
	m_currentFileCount(0),
    m_saveFile(0),
    m_temporaryDir(_saveDir)
{
}

DownloadManager::~DownloadManager() {
	delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QStringList &_urlList)
{
	m_currentFileCount = 0;
	m_currentDownloaderCount = 0;

	// TODO signal pour initialiser la progress bar ?
	// TODO détail du download ?
    foreach (QString url, _urlList) {
		if (m_downloadQueue.isEmpty())
			QTimer::singleShot(0, this, SLOT(startNextDownload()));

		m_downloadQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
		++m_currentFileCount;
	}

    if (m_downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SIGNAL(finished()));
}

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty()) {
        printf("%d/%d files downloaded successfully\n", m_currentDownloaderCount, m_currentFileCount);
        emit finished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();

    QNetworkRequest request(url);
    request.setRawHeader(Global::UserAgentHeader.toLatin1(), Global::UserAgentValue.toLatin1());
    m_currentDownload = m_manager.get(request);

    connect(m_currentDownload, SIGNAL(metaDataChanged()),
            SLOT(metaDataChanged()));
    connect(m_currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateProgress(qint64,qint64)));
    connect(m_currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));
    connect(m_currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));
    connect(m_currentDownload, SIGNAL(error(QNetworkReply::NetworkError)),
			SLOT(errorOccured(QNetworkReply::NetworkError)));
	// TODO redirect

    qDebug() << "Downloading" << url.toEncoded().constData();
    m_downloadTime.start();
}

void DownloadManager::metaDataChanged()
{
    m_currentFilename = "";

    QString contentDisposition = "";
    if (!contentDisposition.isEmpty()) {
        const QString searchString = "filename=";
        int index = contentDisposition.indexOf(searchString, 0, Qt::CaseInsensitive);
        if (index != -1) {
            m_currentFilename = contentDisposition.mid(index + searchString.length());
        }
    }

    if (m_currentFilename.isEmpty()) {
        QString path = m_currentDownload->url().path();
        if (!QFileInfo(path).fileName().isEmpty()) {
            m_currentFilename = QFileInfo(path).fileName();
        }
        else {
            m_currentFilename = "download";
        }
    }

    qDebug() << "out filename:" << m_currentFilename;
    emit downloadFileMessage(m_currentFilename);
    m_saveFile = new QSaveFile(m_temporaryDir + "/" + m_currentFilename);

    qDebug() << "temporary file:" << m_saveFile->fileName();
    if (!m_saveFile->open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening temporary file for URL: " << m_currentDownload->url().toEncoded().constData();
        startNextDownload();
        return; // skip this download
    }

}

void DownloadManager::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);

    // calculate the download speed
    double speed = _bytesReceived * 1000 / m_downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "octets/s";
    } else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "ko/s";
    } else {
        speed /= 1024*1024;
        unit = "Mo/s";
    }

	emit downloadSpeedMessage(QString::fromLatin1("%1 %2")
                            .arg(speed, 3, 'f', 1).arg(unit));
}

void DownloadManager::downloadFinished()
{
    // TODO progressBar.clear();
    emit downloadSpeedMessage("");
	m_saveFile->commit();

	delete m_saveFile;
	m_saveFile = 0;

    if (m_currentDownload->error()) {
        // download failed
        fprintf(stderr, "Failed: %s\n", qPrintable(m_currentDownload->errorString()));
    } else {
        printf("Succeeded.\n");
        ++m_currentDownloaderCount;
    }

    m_currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    m_saveFile->write(m_currentDownload->readAll());
}

void DownloadManager::errorOccured(QNetworkReply::NetworkError _error) {
	qDebug() << _error;
	m_saveFile->cancelWriting();
}

