#ifndef UPDATER__DOWNLOADMANAGER_H
#define UPDATER__DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include "xml/data/application.h"

class QNetworkReply;
class QSaveFile;
class QAuthenticator;

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(const QDir &_temporaryDir, const QUrl &_baseUrl, const QNetworkProxy &_proxy, QObject * parent = 0);
    virtual ~DownloadManager();

    void setUrlListToDownload(const QMap<Application, QList<QUrl> > &_downloads);
    void setUrlListToDownload(const QMap<Application, QList<QString> > &_downloads);

    QSet<QPair<Application, QUrl> > getUrlsInError() const;

    static const short MaxAttemptNumber = 3;

signals:
    void downloadsFinished();
    void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
    void downloadSpeedMessage(const QString &_speed);
    void remainingTimeMessage(const QString &_time);
    void downloadFileMessage(const QString &_file);

    // progression totale
    void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

private slots:

    void slotAuthenticationRequired(QNetworkReply * _reply, QAuthenticator * _authenticator);
    void slotProxyAuthenticationRequired(const QNetworkProxy &_proxy, QAuthenticator * _authenticator);

    void startNextHeadRequest();
    void headMetaDataChanged();
    void currentHeadFinished();

    void startNextDownload();
    void downloadMetaDataChanged();
    void currentDownloadFinished();
    void downloadReadyRead();
    void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);

private:

    void headsFinished();

    static bool createDirIfNotExists(const QDir &_dir);
    static QString getFilenameAndCreateRequiredDirectories(const QUrl &_baseUrl, const QNetworkReply * const _reply, const QDir &_tempDir);

    // url of deployment
    QUrl m_baseUrl;

    QNetworkAccessManager m_manager;

    qint64 m_totalBytesToDownload;
    qint64 m_totalBytesDownloaded;

    short m_currentAttempt;
    short m_currentAuthAttempt;

    QSet<QPair<Application, QUrl> > m_errorSet;

    Application m_currentApplication;
    QNetworkReply * m_currentReply;

    QString m_appName;

    QQueue<QPair<Application, QUrl> > m_headQueue;
    QQueue<QPair<Application, QUrl> > m_downloadQueue;

    QSaveFile * m_saveFile;
    QDir m_temporaryDir;

    /// download time to calculate download speed.
    QTime m_currentDownloadTime;
    QTime m_totalDownloadTime;
    QTime m_lastSampleTime;

    bool m_httpAuthCanceled;
};

#endif // ifndef UPDATER__DOWNLOADMANAGER_H
