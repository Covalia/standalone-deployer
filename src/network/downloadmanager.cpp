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
// TODO : gérer les mauvais hôtes + barre téléchargement.
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
    m_needGlobalProgress(false)
{
}

DownloadManager::~DownloadManager() {
	delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QStringList &_urlList, bool _needGlobalProgress)
{
    m_needGlobalProgress = _needGlobalProgress;
    m_totalBytesToDownload = 0;
    m_totalBytesDownloaded = 0;

    // la file doit être vide !
    if (m_downloadQueue.isEmpty()) {

        // réinitialisation des fichiers en error
        m_errorQueue.clear();
        m_headQueue.clear();

        foreach (QString url, _urlList) {
            m_headQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
            m_downloadQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
        }

        // lancement les requêtes head
        if (_needGlobalProgress) {
            qDebug() << "Need progression, head requests first";
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
        if (m_errorQueue.isEmpty()) {
            qDebug() << "All files downloaded successfully";
        }
        else {
            qDebug() << "Some files could not be downloaded";
            QList<QUrl>::const_iterator it;
            for (it = m_errorQueue.constBegin(); it != m_errorQueue.constEnd(); ++it) {
                const QUrl url = *it;
                qDebug() << "Not downloaded:" << url.toEncoded().constData();
            }
        }
        emit downloadsFinished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();
    qDebug() << "Downloading" << url.toEncoded().constData();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, Global::UserAgentValue);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));
    m_currentDownload = m_manager.get(request);

    connect(m_currentDownload, SIGNAL(metaDataChanged()),
            SLOT(downloadMetaDataChanged()));
    connect(m_currentDownload, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(updateProgress(qint64, qint64)));
    connect(m_currentDownload, SIGNAL(finished()),
            SLOT(currentDownloadFinished()));
    connect(m_currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));
//    connect(m_currentDownload, SIGNAL(error(QNetworkReply::NetworkError)),
//            SLOT(downloadErrorOccured(QNetworkReply::NetworkError)));

    m_downloadTime.start();

}

void DownloadManager::downloadMetaDataChanged()
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
    QVariant statusVariant = m_currentHead->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusVariant.toInt();
    QUrl currentUrl = m_currentHead->url();
    qDebug() << "Status:" << status << "-" << currentUrl.toEncoded().constData();

    QVariant urlVariant = m_currentHead->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (urlVariant.isValid()) {
        QUrl redirectedUrl = urlVariant.toUrl();
        qDebug() << "Redirected to:" << redirectedUrl.toEncoded().constData();
        // on est redirigé
        if (!m_downloadQueue.isEmpty()) {
            // on supprime le premier élément de la file download, pour éviter d'avoir à rejouer les redirections.
            m_downloadQueue.removeFirst();
        }
        m_downloadQueue.prepend(redirectedUrl);
        m_headQueue.prepend(redirectedUrl);
    }
    else {
        // ceci n'est pas une redirection, ajout de la taille du téléchargement au compteur
        if (status == 200) {
            qint64 contentLength = m_currentHead->header(QNetworkRequest::ContentLengthHeader).toString().toLongLong();
            qDebug() << "Head Request for:" << m_currentHead->url().toEncoded().constData() << "- Content-Length:" << contentLength;
            m_mapUrlContentLength.insert(m_currentHead->url(), contentLength);
            m_totalBytesToDownload += contentLength;
        }
    }

}

void DownloadManager::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);
    if (m_needGlobalProgress) {
        emit totalDownloadProgress(m_totalBytesDownloaded, m_totalBytesToDownload);
    }
    else {
        emit totalDownloadProgress(0, 0);
    }

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

    if (m_currentDownload->error()) {
        // download failed
        qDebug() << "Failed:" << qPrintable(m_currentDownload->errorString());
        m_errorQueue.enqueue(m_currentDownload->url());
    }
    else {
        qDebug() << "Success.";
        m_saveFile->commit();
    }

    delete m_saveFile;
    m_saveFile = 0;

    m_currentDownload->deleteLater();
    startNextDownload();
    return;
}

void DownloadManager::downloadReadyRead()
{
    m_totalBytesDownloaded += m_saveFile->write(m_currentDownload->readAll());
}

void DownloadManager::downloadErrorOccured(QNetworkReply::NetworkError _error) {
    qDebug() << "Download request error occured" << _error;
}

void DownloadManager::headErrorOccured(QNetworkReply::NetworkError _error) {
    QUrl url = m_downloadQueue.dequeue();
    qDebug() << "Remove" << url.toEncoded().constData() << "from download queue because of error:" << _error;
    // La requête head a échoué, alors inutile de démarrer le téléchargement...
    m_errorQueue.enqueue(url);
}

void DownloadManager::startNextHeadRequest() {

    if (m_headQueue.isEmpty()) {
        headsFinished();
        return;
    }

    QUrl url = m_headQueue.dequeue();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, Global::UserAgentValue);
    m_currentHead = m_manager.head(request);

    connect(m_currentHead, SIGNAL(metaDataChanged()),
            SLOT(headMetaDataChanged()));
    connect(m_currentHead, SIGNAL(finished()),
            SLOT(currentHeadFinished()));
    connect(m_currentHead, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(headErrorOccured(QNetworkReply::NetworkError)));

}

void DownloadManager::headsFinished() {
    // les requêtes head sont terminées, on passe aux téléchargements
    QTimer::singleShot(0, this, SLOT(startNextDownload()));
}

QQueue<QUrl> DownloadManager::getErrorQueue() const
{
    return m_errorQueue;
}
