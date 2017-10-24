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

// TODO : gérer les erreurs 404.
// TODO : gérer les coupures.
// TODO : gérer les mauvaises URLs.
// TODO : gérer les redirections.
// TODO : gérer les proxies.
// TODO : gérer les authentifications HTTP.
// TODO : gérer les certificats HTTPS invalides.
// TODO : estimer le temps restant.
// TODO : gérer l'écriture des fichiers dans une arborescence temporaire plutôt qu'à la racine.

DownloadManager::DownloadManager(const QString &_saveDir, QObject *_parent) : QObject(_parent),
    m_totalBytesToDownload(0),
    m_totalBytesDownloaded(0),
    m_currentHead(0),
    m_currentDownload(0),
    m_saveFile(0),
    m_temporaryDir(_saveDir),
    m_needProgress(false)
{
}

DownloadManager::~DownloadManager() {
	delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QStringList &_urlList, bool _needProgress)
{
    m_needProgress = _needProgress;
    m_totalBytesToDownload = 0;
    m_totalBytesDownloaded = 0;

    // la file doit être vide !
    if (m_downloadQueue.isEmpty() && m_headQueue.isEmpty()) {
        foreach (QString url, _urlList) {
            m_headQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
            m_downloadQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
        }

        // lancement les requêtes head
        if (_needProgress) {
            qDebug() << "Need progression, head requests first.";
            QTimer::singleShot(0, this, SLOT(startNextHeadRequest()));
        }
        else {
            QTimer::singleShot(0, this, SLOT(startNextDownload()));
        }


        if (m_downloadQueue.isEmpty() && m_headQueue.isEmpty())
            QTimer::singleShot(0, this, SIGNAL(downloadsFinished()));
    }

}

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty()) {
        qDebug() << "All files downloaded successfully";
        emit downloadsFinished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();
    qDebug() << "Downloading" << url.toEncoded().constData();

    QNetworkRequest request(url);
    request.setRawHeader(Global::UserAgentHeader.toLatin1(), Global::UserAgentValue.toLatin1());
    m_currentDownload = m_manager.get(request);

    connect(m_currentDownload, SIGNAL(metaDataChanged()),
            SLOT(metaDataChanged()));
    connect(m_currentDownload, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(updateProgress(qint64, qint64)));
    connect(m_currentDownload, SIGNAL(finished()),
            SLOT(currentDownloadFinished()));
    connect(m_currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));
    connect(m_currentDownload, SIGNAL(error(QNetworkReply::NetworkError)),
			SLOT(errorOccured(QNetworkReply::NetworkError)));

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

    emit downloadFileMessage(m_currentFilename);
    m_saveFile = new QSaveFile(m_temporaryDir + "/" + m_currentFilename);

    qDebug() << "Saving to temporary file:" << m_saveFile->fileName();
    if (!m_saveFile->open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening temporary file for URL: " << m_currentDownload->url().toEncoded().constData();
        startNextDownload();
        return;
    }

}

void DownloadManager::headMetaDataChanged()
{
    qint64 contentLength = QString(m_currentHead->rawHeader(Global::ContentLengthHeader.toLatin1())).toLongLong();
    qDebug() << "Head Request for:" << m_currentHead->url().toEncoded().constData() << Global::ContentLengthHeader << contentLength;
    m_mapUrlContentLength.insert(m_currentHead->url(), contentLength);
    m_totalBytesToDownload += contentLength;

}

void DownloadManager::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);

    // calculate the download speed
    double speed = _bytesReceived * 1000 / m_downloadTime.elapsed();
    QString unit;
    if (speed < 1024) {
        unit = "octets/s";
    }
    else if (speed < 1024*1024) {
        speed /= 1024;
        unit = "ko/s";
    }
    else {
        speed /= 1024*1024;
        unit = "Mo/s";
    }

	emit downloadSpeedMessage(QString::fromLatin1("%1 %2")
                            .arg(speed, 3, 'f', 1).arg(unit));
}

void DownloadManager::currentHeadFinished()
{
    m_currentHead->deleteLater();
    startNextHeadRequest();
}

void DownloadManager::currentDownloadFinished()
{
    emit downloadSpeedMessage("");
	m_saveFile->commit();

	delete m_saveFile;
	m_saveFile = 0;

    if (m_currentDownload->error()) {
        // download failed
        qDebug() << "Failed:" << qPrintable(m_currentDownload->errorString());
    }
    else {
        qDebug() << "Success.";
    }

    m_currentDownload->deleteLater();
    startNextDownload();
    return;
}

void DownloadManager::downloadReadyRead()
{
    m_totalBytesDownloaded += m_saveFile->write(m_currentDownload->readAll());
    if (m_needProgress) {
        emit totalDownloadProgress(m_totalBytesDownloaded, m_totalBytesToDownload);
    }
}

void DownloadManager::errorOccured(QNetworkReply::NetworkError _error) {
	qDebug() << _error;
	m_saveFile->cancelWriting();
}

void DownloadManager::startNextHeadRequest() {

    if (m_headQueue.isEmpty()) {
        headsFinished();
        return;
    }

    QUrl url = m_headQueue.dequeue();
    QNetworkRequest request(url);
    request.setRawHeader(Global::UserAgentHeader.toLatin1(), Global::UserAgentValue.toLatin1());
    m_currentHead = m_manager.head(request);

    connect(m_currentHead, SIGNAL(metaDataChanged()),
            SLOT(headMetaDataChanged()));
    connect(m_currentHead, SIGNAL(finished()),
            SLOT(currentHeadFinished()));

}

void DownloadManager::headsFinished() {
    QTimer::singleShot(0, this, SLOT(startNextDownload()));
}
