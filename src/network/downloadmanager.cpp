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

// TODO : gérer les coupures.
// TODO : gérer plusieurs tentatives de téléchargement ??
// TODO : gérer les mauvais hôtes + barre téléchargement.
// TODO : gérer les proxies.
// TODO : gérer les authentifications HTTP.
// TODO : gérer l'écriture des fichiers dans une arborescence temporaire plutôt qu'à la racine.

DownloadManager::DownloadManager(const QString &_saveDir, QObject *_parent) : QObject(_parent),
    m_totalBytesToDownload(0),
    m_totalBytesDownloaded(0),
    m_currentHead(0),
    m_currentDownload(0),
    m_saveFile(0),
    m_temporaryDir(_saveDir)
{
}

DownloadManager::~DownloadManager() {
	delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QStringList &_urlList)
{
    m_totalBytesToDownload = 0;
    m_totalBytesDownloaded = 0;

    // la file doit être vide pour ne pas écraser d'autres downloads en cours !
    if (m_downloadQueue.isEmpty()) {

        // réinitialisation des fichiers en error
        m_errorSet.clear();
        // réinitialisation de la file d'entêtes à récupérer
        m_headQueue.clear();

        foreach (QString url, _urlList) {
            m_headQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
            m_downloadQueue.enqueue(QUrl::fromEncoded(url.toLocal8Bit()));
        }

        // lancement les requêtes head
        QTimer::singleShot(0, this, SLOT(startNextHeadRequest()));
    }

}

QSet<QUrl> DownloadManager::getUrlsInError() const
{
    return m_errorSet;
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
    m_currentHead->ignoreSslErrors();

    connect(m_currentHead, SIGNAL(metaDataChanged()),
            SLOT(headMetaDataChanged()));
    connect(m_currentHead, SIGNAL(finished()),
            SLOT(currentHeadFinished()));

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
            // on supprime l'élément de la file download, pour éviter d'avoir à rejouer les redirections.
            if (m_downloadQueue.removeOne(currentUrl)) {
                qDebug() << "Remove" << currentUrl.toEncoded().constData() << "from download queue because of redirection";
            }
        }
        qDebug() << "Adding" << redirectedUrl.toEncoded().constData() << "to download queue because of redirection";
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

void DownloadManager::currentHeadFinished()
{
    if (m_currentHead->error()) {
        QUrl url = m_currentHead->url();
        // La requête head a échoué, alors inutile de démarrer le téléchargement...
        if (m_downloadQueue.removeOne(url)) {
            qDebug() << "Remove" << url.toEncoded().constData() << "from download queue because of error:" << m_currentHead->errorString();
        }
        // ajout dans la liste d'erreurs
        m_errorSet.insert(url);
    }

    m_currentHead->deleteLater();
    startNextHeadRequest();
}

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty()) {
        if (m_errorSet.isEmpty()) {
            qDebug() << "All files downloaded successfully";
        }
        else {
            qDebug() << "Some files could not be downloaded";
            QSet<QUrl>::const_iterator it;
            for (it = m_errorSet.constBegin(); it != m_errorSet.constEnd(); ++it) {
                const QUrl url = *it;
                qDebug() << "Not downloaded:" << url.toEncoded().constData();
            }
        }

        emit downloadSpeedMessage("");
        emit remainingTimeMessage("");

        emit downloadsFinished();
        return;
    }

    QUrl url = m_downloadQueue.dequeue();
    qDebug() << "Downloading" << url.toEncoded().constData();

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, Global::UserAgentValue);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));
    m_currentDownload = m_manager.get(request);
    m_currentDownload->ignoreSslErrors();

    connect(m_currentDownload, SIGNAL(metaDataChanged()),
            SLOT(downloadMetaDataChanged()));
    connect(m_currentDownload, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(updateProgress(qint64, qint64)));
    connect(m_currentDownload, SIGNAL(finished()),
            SLOT(currentDownloadFinished()));
    connect(m_currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    m_currentDownloadTime.start();

}

void DownloadManager::downloadMetaDataChanged()
{
    m_currentFilename = "";

    QString contentDisposition = m_currentDownload->rawHeader("Content-Disposition");
    qDebug() << "Content-Disposition:" << contentDisposition;
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
    }

}

void DownloadManager::currentDownloadFinished()
{

    if (m_currentDownload->error()) {
        // download failed
        qDebug() << "Failed:" << qPrintable(m_currentDownload->errorString());
        m_errorSet.insert(m_currentDownload->url());
        if (m_saveFile) {
            m_saveFile->cancelWriting();
        }
    }
    else {
        qDebug() << "Success.";
        m_saveFile->commit();
    }

    delete m_saveFile;
    m_saveFile = 0;

    m_currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    m_totalBytesDownloaded += m_saveFile->write(m_currentDownload->readAll());
}

void DownloadManager::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);
    emit totalDownloadProgress(m_totalBytesDownloaded, m_totalBytesToDownload);

    // on met à jour la vitesse et temps restant une fois par seconde, au plus
    if (m_lastSampleTime.elapsed() > 1000) {

        // calculate the download speed
        double speed = _bytesReceived * 1000 / m_currentDownloadTime.elapsed();

        QString unit;
        if (speed < 1024) {
            //: This string refers to bytes per second.
            unit = tr("o/s");
        }
        else if (speed < 1024 * 1024) {
            speed /= 1024;
            //: This string refers to kilobytes per second.
            unit = tr("ko/s");
        }
        else {
            speed /= 1024 * 1024;
            //: This string refers to megabytes per second.
            unit = tr("Mo/s");
        }

        //: This string refers to download speed as "12 ko/s".
        emit downloadSpeedMessage(tr("%1 %2").arg(speed, 3, 'f', 1).arg(unit));

        qint64 averageSpeed = m_totalBytesDownloaded / m_totalDownloadTime.elapsed();
        qint64 remainingBytesToDownload = m_totalBytesToDownload - m_totalBytesDownloaded;

        int remainingSeconds = remainingBytesToDownload / averageSpeed / 1000;
        int hours = remainingSeconds / 3600;
        int minutes = (remainingSeconds - 3600 * hours) / 60;
        int seconds = remainingSeconds - 3600 * hours - 60 * minutes;

        //: This string refers a time (hours, minutes, seconds).
        emit remainingTimeMessage(tr("%1 %2 %3")
                                  .arg(tr("%n heure(s)", "les heures estimées", hours))
                                  .arg(tr("%n minute(s)", "les minutes estimées", minutes))
                                  .arg(tr("%n seconde(s)", "les secondes estimées", seconds)));

        m_lastSampleTime.restart();
    }

}

void DownloadManager::headsFinished() {
    // les requêtes head sont terminées, on passe aux téléchargements
    m_totalDownloadTime.start();
    m_lastSampleTime.start();
    QTimer::singleShot(0, this, SLOT(startNextDownload()));
}
