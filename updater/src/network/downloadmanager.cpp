#include "network/downloadmanager.h"
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QSaveFile>
#include <QDir>
#include "updater/config.h"
#include "ui_authenticationdialog.h"
#include "log/logger.h"
#include "xml/data/application.h"

// TODO : gérer les coupures dans le download + head.
// TODO : gérer plusieurs tentatives de téléchargement + réinitialiser barre globale en cas de retry ??
// TODO : gérer les mauvais hôtes + barre téléchargement.
// TODO : détecter authentification proxy réussie.
// TODO : tester auth proxy
// TODO : gérer les redirections sur l'URL de base
// TODO : refaire un debug / refactor global concernant les codes d'erreurs, proxies.

DownloadManager::DownloadManager(const QDir &_temporaryDir, const QUrl &_baseUrl, const QNetworkProxy &_proxy, QObject * _parent) : QObject(_parent),
    m_baseUrl(_baseUrl),
    m_totalBytesToDownload(0),
    m_totalBytesDownloaded(0),
    m_currentAttempt(0),
    m_currentApplication(Application::getEmptyApplication()),
    m_currentReply(nullptr),
    m_saveFile(nullptr),
    m_temporaryDir(_temporaryDir)
{
    L_INFO(QString("Creating DownloadManager with base URL: %1").arg(_baseUrl.toString()));
    L_INFO(QString("Temporary directory: %1").arg(_temporaryDir.absolutePath()));

    // définition du proxy si host et port définis
    if (!_proxy.hostName().isEmpty() && _proxy.port() > 0) {
        L_INFO(QString("Proxy to use: %1:%2").arg(_proxy.hostName()).arg(_proxy.port()));
        QNetworkProxy::setApplicationProxy(_proxy);

        connect(&m_manager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&,QAuthenticator*)),
                SLOT(onProxyAuthenticationRequired(const QNetworkProxy&,QAuthenticator*)));
    } else {
        L_INFO("No proxy.");
    }
}

DownloadManager::~DownloadManager()
{
    delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QMultiMap<Application, QUrl> &_downloadsMap)
{
    // la file doit être vide pour ne pas écraser d'autres downloads en cours !
    if (m_downloadQueue.isEmpty()) {
        m_totalBytesToDownload = 0;
        m_totalBytesDownloaded = 0;
        m_currentAttempt = 0;

        // réinitialisation des fichiers en error
        m_errorSet.clear();
        // réinitialisation de la file d'entêtes à récupérer
        m_headQueue.clear();

        QMapIterator<Application, QUrl> iterator(_downloadsMap);
        while (iterator.hasNext()) {
            iterator.next();
            const Application & application = iterator.key();
            const QUrl url = iterator.value();

            // concatenation base avec url
            const QUrl buildUrl = m_baseUrl.resolved(application.getName() + "/").resolved(url);

            L_INFO(QString("%1 - build URL: %2").arg(application.getName()).arg(buildUrl.toString()));
            QPair<Application, QUrl> pair(application, buildUrl);
            m_headQueue.enqueue(pair);
            m_downloadQueue.enqueue(pair);
        }

        // lancement les requêtes head
        QTimer::singleShot(0, this, SLOT(startNextHeadRequest()));
    } else {
        L_WARN("Download queue is not empty, cannot add to queue.");
    }
}

void DownloadManager::setUrlListToDownload(const QMultiMap<Application, QString> &_downloadsMap)
{
    QMultiMap<Application, QUrl> urlMap;

    L_INFO("Converting QMultiMap<Application, QString> to QMultiMap<Application, QUrl>.");

    QMapIterator<Application, QString> iterator(_downloadsMap);
    while (iterator.hasNext()) {
        iterator.next();
        const Application & application = iterator.key();
        const QString & url = iterator.value();

        urlMap.insert(application, QUrl(url));
    }

    setUrlListToDownload(urlMap);
}

QSet<QPair<Application, QUrl> > DownloadManager::getUrlsInError() const
{
    return m_errorSet;
}

void DownloadManager::onProxyAuthenticationRequired(const QNetworkProxy &_proxy, QAuthenticator * _authenticator)
{
    L_INFO("Proxy Authentication required");
    QDialog authenticationDialog;
    Ui::Dialog ui;
    ui.setupUi(&authenticationDialog);
    authenticationDialog.setWindowTitle(tr("Proxy authentication required"));
    authenticationDialog.adjustSize();
    ui.siteDescription->setText(tr("%1", "The proxy domain").arg(_authenticator->realm()));

    ui.userEdit->setText(_proxy.user());
    ui.passwordEdit->setText(_proxy.password());

    if (authenticationDialog.exec() == QDialog::Accepted) {
        L_INFO(QString("Authentication with user: %1").arg(ui.userEdit->text()));

        _authenticator->setUser(ui.userEdit->text());
        _authenticator->setPassword(ui.passwordEdit->text());
    } else {
        // annuler tout (vider toutes les queues)
        L_INFO("Aborting all downloads, canceled by user.");

        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;

        // tous les fichiers restants seront en erreur, car même serveur.
        m_errorSet += m_headQueue.toSet();
        // on vide les files
        m_downloadQueue.clear();
        m_headQueue.clear();
    }
}

void DownloadManager::startNextHeadRequest()
{
    if (m_headQueue.isEmpty()) {
        headsFinished();
        return;
    }

    QPair<Application, QUrl> pair = m_headQueue.dequeue();
    m_currentApplication = pair.first;
    QNetworkRequest request(pair.second);
    request.setHeader(QNetworkRequest::UserAgentHeader, UpdaterConfig::UserAgentValue);
    m_currentReply = m_manager.head(request);
    m_currentReply->ignoreSslErrors();

    connect(m_currentReply, SIGNAL(metaDataChanged()),
            SLOT(headMetaDataChanged()));
    connect(m_currentReply, SIGNAL(finished()),
            SLOT(currentHeadFinished()));
}

void DownloadManager::headMetaDataChanged()
{
    QVariant statusVariant = m_currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int status = statusVariant.toInt();

    QPair<Application, QUrl> currentUrlPair(m_currentApplication, m_currentReply->url());

    L_INFO(QString("Status: %1 - %2").arg(QString::number(status)).arg(currentUrlPair.second.toEncoded().constData()));

    QVariant urlVariant = m_currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (urlVariant.isValid()) {
        QPair<Application, QUrl> redirectedUrlPair(m_currentApplication, urlVariant.toUrl());
        L_INFO(QString("Redirected to: %1").arg(QString(redirectedUrlPair.second.toEncoded().constData())));
        // on est redirigé
        if (!m_downloadQueue.isEmpty()) {
            // on supprime l'élément de la file download, pour éviter d'avoir à rejouer les redirections.
            if (m_downloadQueue.removeOne(currentUrlPair)) {
                L_INFO(QString("Remove %1 from download queue because of redirection").arg(QString(currentUrlPair.second.toEncoded().constData())));
            }
        }
        L_INFO(QString("Adding %1 to download queue because of redirection").arg(QString(redirectedUrlPair.second.toEncoded().constData())));
        m_downloadQueue.prepend(redirectedUrlPair);
        m_headQueue.prepend(redirectedUrlPair);
    } else {
        // ceci n'est pas une redirection, ajout de la taille du téléchargement au compteur
        if (status == 200) {
            qint64 contentLength = m_currentReply->header(QNetworkRequest::ContentLengthHeader).toString().toLongLong();
            L_INFO(QString("Head Request for: %1 - Content-Length: %2").arg(QString(m_currentReply->url().toEncoded().constData())).arg(QString::number(contentLength)));
            m_totalBytesToDownload += contentLength;
        }
    }
}

void DownloadManager::currentHeadFinished()
{
    if (m_currentReply->error()) {
        QPair<Application, QUrl> pair(m_currentApplication, m_currentReply->url());

        if (m_currentAttempt >= MaxAttemptNumber - 1) {
            // ajout dans la liste d'erreurs
            m_errorSet.insert(pair);
            // La requête head a échoué, alors inutile de démarrer le téléchargement...
            if (m_downloadQueue.removeOne(pair)) {
                L_ERROR(QString("Remove %1 from download queue because of error: %2").arg(QString(pair.second.toEncoded().constData())).arg(m_currentReply->errorString()));
            }
        } else {
            // on rajoute l'url dans le head pour refaire une tentative
            L_ERROR(QString("Tried: %1 but error occured: %2").arg(QString(pair.second.toEncoded().constData())).arg(m_currentReply->errorString()));
            if (m_currentReply->error() != QNetworkReply::NetworkError::OperationCanceledError
                && m_currentReply->error() != QNetworkReply::NetworkError::ProxyAuthenticationRequiredError) {
                L_INFO(QString("Prepend URL: %1 because of error: %2").arg(pair.second.toString()).arg(m_currentReply->error()));
                // pour tout autre type d'erreur, on remet en file
                m_headQueue.prepend(pair);
            } else {
                m_errorSet.insert(pair);
            }
            ++m_currentAttempt;
        }
    } else {
        m_currentAttempt = 0;
    }

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
    startNextHeadRequest();
}

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty()) {
        if (m_errorSet.isEmpty()) {
            L_INFO("All files downloaded successfully");
        } else {
            L_ERROR("Some files could not be downloaded");
            QSet<QPair<Application, QUrl> >::const_iterator it;
            for (it = m_errorSet.constBegin(); it != m_errorSet.constEnd(); ++it) {
                const QPair<Application, QUrl> pair = *it;
                L_ERROR(QString("Not downloaded: %1").arg(QString(pair.second.toEncoded().constData())));
            }
        }

        emit downloadSpeedMessage("");
        emit remainingTimeMessage("");

        emit downloadsFinished();
        return;
    }

    QPair<Application, QUrl> pair = m_downloadQueue.dequeue();
    m_currentApplication = pair.first;
    L_INFO(QString("Downloading %1").arg(QString(pair.second.toEncoded().constData())));

    QNetworkRequest request(pair.second);
    request.setHeader(QNetworkRequest::UserAgentHeader, UpdaterConfig::UserAgentValue);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, QVariant(true));
    m_currentReply = m_manager.get(request);
    m_currentReply->ignoreSslErrors();

    connect(m_currentReply, SIGNAL(metaDataChanged()),
            SLOT(downloadMetaDataChanged()));
    connect(m_currentReply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateProgress(qint64,qint64)));
    connect(m_currentReply, SIGNAL(finished()),
            SLOT(currentDownloadFinished()));
    connect(m_currentReply, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    m_currentDownloadTime.start();
}

void DownloadManager::downloadMetaDataChanged()
{
    QString currentFilename = getFilenameAndCreateRequiredDirectories(m_baseUrl, m_currentReply, m_temporaryDir);

    emit downloadFileMessage(QFileInfo(currentFilename).fileName());

    m_saveFile = new QSaveFile(currentFilename);

    L_INFO(QString("Saving to temporary file: %1").arg(m_saveFile->fileName()));
    if (currentFilename.isEmpty() || !m_saveFile->open(QIODevice::WriteOnly)) {
        L_ERROR(QString("Error opening temporary file for URL: %1").arg(QString(m_currentReply->url().toEncoded().constData())));
        m_errorSet.insert(QPair<Application, QUrl>(m_currentApplication, m_currentReply->url()));
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
        // réinit compteur tentatives
        m_currentAttempt = 0;
        startNextDownload();
    }
}

void DownloadManager::currentDownloadFinished()
{
    if (m_currentReply->error()) {
        QPair<Application, QUrl> pair(m_currentApplication, m_currentReply->url());

        L_ERROR(QString("Failed: %1").arg(m_currentReply->errorString()));
        if (m_currentAttempt >= MaxAttemptNumber - 1) {
            // download failed
            m_errorSet.insert(pair);
        } else {
            // on rajoute l'url dans le download pour refaire une tentative
            ++m_currentAttempt;
            m_downloadQueue.prepend(pair);
        }

        // dans tous les cas, on cancel le fichier, on va le recréer
        if (m_saveFile) {
            m_saveFile->cancelWriting();
        }
    } else {
        L_INFO("Success.");
        m_saveFile->commit();
        m_currentAttempt = 0;
    }

    delete m_saveFile;
    m_saveFile = nullptr;

    m_currentReply->deleteLater();
    m_currentReply = nullptr;
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    m_totalBytesDownloaded += m_saveFile->write(m_currentReply->readAll());
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
            unit = tr("B/s");
        } else if (speed < 1024 * 1024) {
            speed /= 1024;
            //: This string refers to kilobytes per second.
            unit = tr("kB/s");
        } else {
            speed /= 1024 * 1024;
            //: This string refers to megabytes per second.
            unit = tr("MB/s");
        }

        //: This string refers to download speed as "12 ko/s".
        emit downloadSpeedMessage(tr("%1 %2").arg(speed, 3, 'f', 1).arg(unit));

        const qint64 averageSpeed = m_totalBytesDownloaded / m_totalDownloadTime.elapsed();
        const qint64 remainingBytesToDownload = m_totalBytesToDownload - m_totalBytesDownloaded;

        if (averageSpeed != 0) {
            const int remainingSeconds = static_cast<int>(remainingBytesToDownload / averageSpeed / 1000);
            const int hours = remainingSeconds / 3600;
            const int minutes = (remainingSeconds - 3600 * hours) / 60;
            const int seconds = remainingSeconds - 3600 * hours - 60 * minutes;

            const QString hoursStr = tr("%n hour(s)", "the estimated heures", hours);
            const QString minutesStr = tr("%n minute(s)", "the estimated minutes", minutes);
            const QString secondsStr = tr("%n second(s)", "the estimated seconds", seconds);

            QString time;
            if (hours == 0) {
                if (minutes == 0) {
                    if (seconds == 0) {
                        time = "";
                    } else {
                        time = tr("%1", "seconds only").arg(secondsStr);
                    }
                } else {
                    if (seconds == 0) {
                        time = tr("%1", "minutes only").arg(minutesStr);
                    } else {
                        time = tr("%1 %2", "minutes and seconds only").arg(minutesStr).arg(secondsStr);
                    }
                }
            } else {
                if (minutes == 0) {
                    if (seconds == 0) {
                        time = tr("%1", "hours only").arg(hoursStr);
                    } else {
                        time = tr("%1 %2", "hours and seconds only").arg(hoursStr).arg(secondsStr);
                    }
                } else {
                    if (seconds == 0) {
                        time = tr("%1 %2", "hours and minutes only").arg(hoursStr).arg(minutesStr);
                    } else {
                        time = tr("%1 %2 %3", "hours minutes seconds").arg(hoursStr).arg(minutesStr).arg(secondsStr);
                    }
                }
            }

            emit remainingTimeMessage(time);
        }
        m_lastSampleTime.restart();
    }
}

void DownloadManager::headsFinished()
{
    // les requêtes head sont terminées, on passe aux téléchargements
    m_totalDownloadTime.start();
    m_lastSampleTime.start();

    m_currentAttempt = 0;
    QTimer::singleShot(0, this, SLOT(startNextDownload()));
}

bool DownloadManager::createDirIfNotExists(const QDir &_dir)
{
    if (_dir.exists()) {
        return true;
    } else {
        bool created = QDir().mkpath(_dir.absolutePath());
        if (created) {
            L_INFO(QString("Success while creating parent directory: %1").arg(_dir.absolutePath()));
        } else {
            L_ERROR(QString("Error while creating parent directory: %1").arg(_dir.absolutePath()));
        }
        return created;
    }
}

QString DownloadManager::getFilenameAndCreateRequiredDirectories(const QUrl &_baseUrl, const QNetworkReply * const _reply, const QDir &_tempDir)
{
    const QUrl url = _reply->url();

    const QString relativePath = url.toString().remove(0, _baseUrl.toString().size());

    const QFileInfo fileInfo(_tempDir.absoluteFilePath(relativePath));
    const QDir parentDir = fileInfo.dir();
    const bool created = createDirIfNotExists(parentDir);

    if (created) {
        QString filename = "";

        QString contentDisposition = _reply->rawHeader("Content-Disposition");
        if (!contentDisposition.isEmpty()) {
            L_INFO(QString("Content-Disposition: %1").arg(contentDisposition));
            const QString searchString = "filename=";
            const int index = contentDisposition.indexOf(searchString, 0, Qt::CaseInsensitive);
            if (index != -1) {
                filename = contentDisposition.mid(index + searchString.length());
            }
        }

        if (filename.isEmpty()) {
            const QString path = _reply->url().path();
            if (!QFileInfo(path).fileName().isEmpty()) {
                filename = QFileInfo(path).fileName();
            } else {
                filename = "download";
            }
        }

        return parentDir.absoluteFilePath(filename);
    }

    return "";
}
