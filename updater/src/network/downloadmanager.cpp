#include "network/downloadmanager.h"
#include <QDebug>
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

// TODO : gérer les coupures dans le download + head.
// TODO : gérer plusieurs tentatives de téléchargement + réinitialiser barre globale en cas de retry ??
// TODO : gérer les mauvais hôtes + barre téléchargement.
// TODO : détecter authentification proxy réussie.
// TODO : tester auth proxy + auth http simultanée
// TODO : gérer les redirections sur l'URL de base
// TODO : refaire un debug / refactor global concernant les codes d'erreurs, proxies, http auth.

DownloadManager::DownloadManager(const QDir &_temporaryDir, const QUrl &_baseUrl, const QNetworkProxy &_proxy, QObject * _parent) : QObject(_parent),
    m_baseUrl(_baseUrl),
    m_totalBytesToDownload(0),
    m_totalBytesDownloaded(0),
    m_currentAttempt(0),
    m_currentAuthAttempt(0),
    m_currentReply(0),
    m_appName(""),
    m_saveFile(0),
    m_temporaryDir(_temporaryDir),
    m_httpAuthCanceled(0)
{
    // définition du proxy si host et port définis
    if (!_proxy.hostName().isEmpty() && _proxy.port() > 0) {
        QNetworkProxy::setApplicationProxy(_proxy);

        connect(&m_manager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&,QAuthenticator *)),
                SLOT(slotProxyAuthenticationRequired(const QNetworkProxy&,QAuthenticator *)));
    }

    connect(&m_manager, SIGNAL(authenticationRequired(QNetworkReply *,QAuthenticator *)),
            SLOT(slotAuthenticationRequired(QNetworkReply *,QAuthenticator *)));
}

DownloadManager::~DownloadManager()
{
    delete m_saveFile;
}

void DownloadManager::setUrlListToDownload(const QString &_appName, const QList<QUrl> &_urlList)
{
    // la file doit être vide pour ne pas écraser d'autres downloads en cours !
    if (m_downloadQueue.isEmpty()) {
        m_appName = _appName;
        m_totalBytesToDownload = 0;
        m_totalBytesDownloaded = 0;
        m_currentAttempt = 0;
        m_currentAuthAttempt = 0;
        m_httpAuthCanceled = 0;

        // réinitialisation des fichiers en error
        m_errorSet.clear();
        // réinitialisation de la file d'entêtes à récupérer
        m_headQueue.clear();

        foreach(QUrl url, _urlList) {
            // concatenation base avec url
            qDebug() << "Build URL:" << m_baseUrl.resolved(url).toString();
            m_headQueue.enqueue(m_baseUrl.resolved(url));
            m_downloadQueue.enqueue(m_baseUrl.resolved(url));
        }

        // lancement les requêtes head
        QTimer::singleShot(0, this, SLOT(startNextHeadRequest()));
    }
} // DownloadManager::setUrlListToDownload

QSet<QUrl> DownloadManager::getUrlsInError() const
{
    return m_errorSet;
}

void DownloadManager::slotAuthenticationRequired(QNetworkReply * _reply, QAuthenticator * _authenticator)
{
    if (!m_httpAuthCanceled) {
        qDebug() << "HTTP Authentication required";
        QUrl url = _reply->url();

        if (m_currentAuthAttempt >= MaxAttemptNumber) {
            // annuler tout (vider toutes les queues)
            qDebug() << "Aborting all downloads, too many attempts.";
            _reply->abort();
            _reply->deleteLater();
            _reply = 0;

            // tous les fichiers restants seront en erreur, car même serveur.
            m_errorSet += m_headQueue.toSet();
            // on vide les files de téléchargement
            m_downloadQueue.clear();
            m_headQueue.clear();
        } else {
            QDialog authenticationDialog;
            Ui::Dialog ui;
            ui.setupUi(&authenticationDialog);
            authenticationDialog.setWindowTitle(tr("HTTP authentification required"));
            authenticationDialog.adjustSize();
            ui.siteDescription->setText(tr("%1 on %2", "The realm on host").arg(_authenticator->realm(), url.host()));

            if (authenticationDialog.exec() == QDialog::Accepted) {
                qDebug() << "Authentication with user:" << ui.userEdit->text();

                _authenticator->setUser(ui.userEdit->text());
                _authenticator->setPassword(ui.passwordEdit->text());
            } else {
                // annulation, on stoppe.
                m_httpAuthCanceled = true;

                // annuler tout (vider toutes les queues)
                qDebug() << "Aborting all downloads, canceled by user.";
                _reply->abort();
                _reply->deleteLater();
                _reply = 0;

                // tous les fichiers restants seront en erreur, car même serveur.
                m_errorSet += m_headQueue.toSet();
                // on vide les files
                m_downloadQueue.clear();
                m_headQueue.clear();

                // m_currentAuthAttempt = MaxAttemptNumber;
            }
            ++m_currentAuthAttempt;
        }
    } else {
        qDebug() << "HTTP Authentication required, but already canceled by user";
    }
} // DownloadManager::slotAuthenticationRequired

void DownloadManager::slotProxyAuthenticationRequired(const QNetworkProxy &_proxy, QAuthenticator * _authenticator)
{
    qDebug() << "Proxy Authentication required";
    QDialog authenticationDialog;
    Ui::Dialog ui;
    ui.setupUi(&authenticationDialog);
    authenticationDialog.setWindowTitle(tr("Proxy authentication required"));
    authenticationDialog.adjustSize();
    ui.siteDescription->setText(tr("%1", "The proxy domain").arg(_authenticator->realm()));

    ui.userEdit->setText(_proxy.user());
    ui.passwordEdit->setText(_proxy.password());

    if (authenticationDialog.exec() == QDialog::Accepted) {
        qDebug() << "Authentication with user:" << ui.userEdit->text();

        _authenticator->setUser(ui.userEdit->text());
        _authenticator->setPassword(ui.passwordEdit->text());
    } else {
        // annuler tout (vider toutes les queues)
        qDebug() << "Aborting all downloads, canceled by user.";

        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = 0;

        // tous les fichiers restants seront en erreur, car même serveur.
        m_errorSet += m_headQueue.toSet();
        // on vide les files
        m_downloadQueue.clear();
        m_headQueue.clear();
    }
} // DownloadManager::slotProxyAuthenticationRequired

void DownloadManager::startNextHeadRequest()
{
    if (m_headQueue.isEmpty()) {
        headsFinished();
        return;
    }

    QUrl url = m_headQueue.dequeue();
    QNetworkRequest request(url);
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
    QUrl currentUrl = m_currentReply->url();

    qDebug() << "Status:" << status << "-" << currentUrl.toEncoded().constData();

    QVariant urlVariant = m_currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute);
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
    } else {
        // ceci n'est pas une redirection, ajout de la taille du téléchargement au compteur
        if (status == 200) {
            qint64 contentLength = m_currentReply->header(QNetworkRequest::ContentLengthHeader).toString().toLongLong();
            qDebug() << "Head Request for:" << m_currentReply->url().toEncoded().constData() << "- Content-Length:" << contentLength;
            m_mapUrlContentLength.insert(m_currentReply->url(), contentLength);
            m_totalBytesToDownload += contentLength;
        }
    }
} // DownloadManager::headMetaDataChanged

void DownloadManager::currentHeadFinished()
{
    if (m_currentReply->error()) {
        QUrl url = m_currentReply->url();

        if (m_currentAttempt >= MaxAttemptNumber - 1) {
            // ajout dans la liste d'erreurs
            m_errorSet.insert(url);
            // La requête head a échoué, alors inutile de démarrer le téléchargement...
            if (m_downloadQueue.removeOne(url)) {
                qDebug() << "Remove" << url.toEncoded().constData() << "from download queue because of error:" << m_currentReply->errorString();
            }
        } else {
            // on rajoute l'url dans le head pour refaire une tentative
            qDebug() << "Tried:" << url.toEncoded().constData() << "but error occured:" << m_currentReply->errorString();
            if (m_currentReply->error() != QNetworkReply::NetworkError::OperationCanceledError
                && m_currentReply->error() != QNetworkReply::NetworkError::AuthenticationRequiredError
                && m_currentReply->error() != QNetworkReply::NetworkError::ProxyAuthenticationRequiredError) {
                qDebug() << "Prepend url:" << url << "because of error:" << m_currentReply->error();
                // pour tout autre type d'erreur, on remet en file
                m_headQueue.prepend(url);
            } else {
                m_errorSet.insert(url);
            }
            ++m_currentAttempt;
        }
    } else {
        m_currentAttempt = 0;
    }

    m_currentReply->deleteLater();
    m_currentReply = 0;
    startNextHeadRequest();
} // DownloadManager::currentHeadFinished

void DownloadManager::startNextDownload()
{
    if (m_downloadQueue.isEmpty()) {
        if (m_errorSet.isEmpty()) {
            qDebug() << "All files downloaded successfully";
        } else {
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
} // DownloadManager::startNextDownload

void DownloadManager::downloadMetaDataChanged()
{
    QString currentFilename = getFilenameAndCreateRequiredDirectories(m_baseUrl, m_currentReply, m_temporaryDir, m_appName);

    emit downloadFileMessage(QFileInfo(currentFilename).fileName());

    m_saveFile = new QSaveFile(currentFilename);

    qDebug() << "Saving to temporary file:" << m_saveFile->fileName();
    if (currentFilename.isEmpty() || !m_saveFile->open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening temporary file for URL:" << m_currentReply->url().toEncoded().constData();
        m_errorSet.insert(m_currentReply->url());
        m_currentReply->abort();
        m_currentReply->deleteLater();
        m_currentReply = 0;
        // réinit compteur tentatives
        m_currentAttempt = 0;
        startNextDownload();
    }
} // DownloadManager::downloadMetaDataChanged

void DownloadManager::currentDownloadFinished()
{
    if (m_currentReply->error()) {
        QUrl url = m_currentReply->url();

        qDebug() << "Failed:" << qPrintable(m_currentReply->errorString());
        if (m_currentAttempt >= MaxAttemptNumber - 1) {
            // download failed
            m_errorSet.insert(url);
        } else {
            // on rajoute l'url dans le download pour refaire une tentative
            ++m_currentAttempt;
            m_downloadQueue.prepend(url);
        }

        // dans tous les cas, on cancel le fichier, on va le recréer
        if (m_saveFile) {
            m_saveFile->cancelWriting();
        }
    } else {
        qDebug() << "Success.";
        m_saveFile->commit();
        m_currentAttempt = 0;
    }

    delete m_saveFile;
    m_saveFile = 0;

    m_currentReply->deleteLater();
    m_currentReply = 0;
    startNextDownload();
} // DownloadManager::currentDownloadFinished

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

        qint64 averageSpeed = m_totalBytesDownloaded / m_totalDownloadTime.elapsed();
        qint64 remainingBytesToDownload = m_totalBytesToDownload - m_totalBytesDownloaded;

        if (averageSpeed != 0) {
            int remainingSeconds = remainingBytesToDownload / averageSpeed / 1000;
            int hours = remainingSeconds / 3600;
            int minutes = (remainingSeconds - 3600 * hours) / 60;
            int seconds = remainingSeconds - 3600 * hours - 60 * minutes;

            QString hoursStr = tr("%n hour(s)", "the estimated heures", hours);
            QString minutesStr = tr("%n minute(s)", "the estimated minutes", minutes);
            QString secondsStr = tr("%n second(s)", "the estimated seconds", seconds);

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
} // DownloadManager::updateProgress

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
        bool created = QDir().mkpath(_dir.path());
        if (created) {
            qDebug() << "Success while creating parent directory:" << _dir.path();
        } else {
            qDebug() << "Error while creating parent directory:" << _dir.path();
        }
        return created;
    }
}

QString DownloadManager::getFilenameAndCreateRequiredDirectories(const QUrl &_baseUrl, const QNetworkReply * const _reply, const QDir &_tempDir, const QString &_appName)
{
    const QUrl url = _reply->url();

    QString relativePath = url.toString().remove(0, _baseUrl.toString().size());

    QFileInfo fileInfo(_tempDir.absoluteFilePath(_appName + QDir::separator() + relativePath));
    QDir parentDir = fileInfo.dir();
    bool created = createDirIfNotExists(parentDir);

    if (created) {
        QString filename = "";

        QString contentDisposition = _reply->rawHeader("Content-Disposition");
        if (!contentDisposition.isEmpty()) {
            qDebug() << "Content-Disposition:" << contentDisposition;
            const QString searchString = "filename=";
            int index = contentDisposition.indexOf(searchString, 0, Qt::CaseInsensitive);
            if (index != -1) {
                filename = contentDisposition.mid(index + searchString.length());
            }
        }

        if (filename.isEmpty()) {
            QString path = _reply->url().path();
            if (!QFileInfo(path).fileName().isEmpty()) {
                filename = QFileInfo(path).fileName();
            } else {
                filename = "download";
            }
        }

        return parentDir.absoluteFilePath(filename);
    }

    return "";
} // DownloadManager::getFilenameAndCreateRequiredDirectories
