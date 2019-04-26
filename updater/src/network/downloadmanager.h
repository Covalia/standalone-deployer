#ifndef UPDATER_DOWNLOADMANAGER_H
#define UPDATER_DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include "xml/data/application.h"
#include "network/downloadmanagererror.h"

class QNetworkReply;
class QSaveFile;
class QAuthenticator;
class QTimer;

class DownloadManager : public QObject
{
    Q_OBJECT

    public:
        explicit DownloadManager(const QDir &_temporaryDir, const QUrl &_baseUrl, const QNetworkProxy &_proxy, QWidget * parent = nullptr);
        virtual ~DownloadManager();

        void setUrlListToDownload(const QMultiMap<Application, QUrl> &_downloads);
        void setUrlListToDownload(const QMultiMap<Application, QString> &_downloads);

        static const short MaxAttemptNumber = 3;

    signals:
        void downloadsFinished();
        void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void downloadSpeedMessage(const QString &_speed);
        void remainingTimeMessage(const QString &_time);
        void downloadFileMessage(const QString &_file);

        // progression totale
        void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

        // new proxy credentials filled
        void proxyCredentialsChanged(const QString &_login, const QString &_password);

        void error(DownloadManagerError::ErrorType _error);

    private slots:

        void onProxyAuthenticationRequired(const QNetworkProxy &_proxy, QAuthenticator * _authenticator);

        void startNextHeadRequest();
        void headMetaDataChanged();
        void currentHeadFinished();

        void startNextDownload();
        void downloadMetaDataChanged();
        void currentDownloadFinished();
        void downloadReadyRead();
        void onDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void onHeadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

        void onTimeout();

    private:

        void headsFinished();
        void logFilesInError() const;

        static bool createDirIfNotExists(const QDir &_dir);
        static QString getFilenameAndCreateRequiredDirectories(const QUrl &_baseUrl, const QNetworkReply * const _reply, const QDir &_tempDir);

        QWidget * m_parent;

        // global timer to check for timeout
        QTimer * m_timeoutTimer;

        // url of deployment
        QUrl m_baseUrl;

        QNetworkAccessManager m_manager;

        qint64 m_totalBytesToDownload;
        qint64 m_totalBytesDownloaded;

        short m_currentAttempt;

        QSet<QPair<Application, QUrl> > m_errorSet;

        Application m_currentApplication;
        QNetworkReply * m_currentReply;

        QQueue<QPair<Application, QUrl> > m_headQueue;
        QQueue<QPair<Application, QUrl> > m_downloadQueue;

        QSaveFile * m_saveFile;
        QDir m_temporaryDir;

        /// download time to calculate download speed.
        QTime m_currentDownloadTime;
        QTime m_totalDownloadTime;
        QTime m_lastSampleTime;
};

#endif
