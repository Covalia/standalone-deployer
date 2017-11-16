#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkProxy>

class QSaveFile;
class QAuthenticator;

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    explicit DownloadManager(const QString &_temporaryDir, const QUrl &_baseUrl, const QNetworkProxy &_proxy, QObject * parent = 0);
    virtual ~DownloadManager();

    void setUrlListToDownload(const QString &_appName, const QList<QUrl> &_urlList);

    QSet<QUrl> getUrlsInError() const;

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
    static QString getFilenameAndCreateRequiredDirectories(const QUrl &_baseUrl, const QNetworkReply * const _reply, const QDir &_tempDir, const QString &_appName);

    QUrl m_baseUrl;
    QNetworkAccessManager m_manager;

    qint64 m_totalBytesToDownload;
    qint64 m_totalBytesDownloaded;

    short m_currentAttempt;
    short m_currentAuthAttempt;

    QMap<QUrl, long> m_mapUrlContentLength;

    QSet<QUrl> m_errorSet;

    QNetworkReply * m_currentReply;

    QString m_appName;
    QQueue<QUrl> m_headQueue;
    QQueue<QUrl> m_downloadQueue;

    QSaveFile * m_saveFile;
    QString m_temporaryDir;

    /// download time to calculate download speed.
    QTime m_currentDownloadTime;
    QTime m_totalDownloadTime;
    QTime m_lastSampleTime;

    bool m_httpAuthCanceled;
};

#endif // ifndef DOWNLOADMANAGER_H
