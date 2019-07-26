#ifndef UPDATER_APPUPDATER_H
#define UPDATER_APPUPDATER_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QList>
#include <QMultiMap>
#include "xml/data/application.h"
#include "xml/data/download.h"
#include "network/downloadmanagererror.h"
#include "utils.h"

class DownloadManager;

class AppUpdater : public QObject
{
    Q_OBJECT

    public:
        explicit AppUpdater(const QUrl &_appUrl, QWidget * _parent = nullptr);
        virtual ~AppUpdater();
        void start();

    private slots:
        void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void updateDownloadSpeedMessage(const QString &_speed);
        void updateRemainingTimeMessage(const QString &_time);
        void updateDownloadingFileMessage(const QString &_file);
        void updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void cnlpDownloadFinished();
        void applicationDownloadFinished();
        void updateProxyCredentials(const QString &_login, const QString &_password);
        void handleDownloaderError(DownloadManagerError::ErrorType _error);

    private:

        DownloadManager * m_updater;
        QUrl m_appUrl;
        AppPath m_appPath;

        QString m_remoteUpdaterVersion;
        QString m_localUpdaterVersion;
        QString m_remoteJavaVersion;
        QString m_localJavaVersion;

        // list of remote files to download
        QMultiMap<Application, Download> m_cnlpParsedFiles;
        // list of application files to download, those who have changed
        QMultiMap<Application, QString> m_filesToDownload;
        // list of application files to keep, those who haven't changed
        QMultiMap<Application, QString> m_filesToKeep;
        // list of remaining application files, those who need to be removed
        QMultiMap<Application, QString> m_remainingFiles;

        // encoding to use to start application
        QString m_encoding;
        // main class to start
        QString m_mainClass;
        // post class runner to start
        QString m_runnerClass;

        // received arguments list from cnlp
        QStringList m_arguments;

        // memory to set when starting application
        QString m_memory;

        QList<QString> getLocalFiles(const Application &_application);
        QMultiMap<Application, QString> getLocalFiles();
        void processCnlpDownloadFileList();
        bool buildApplicationInTempDirectory(const Application &_application);

        // return true if the application has changed
        bool doesAppNeedToBeRebuild(const Application &_application);

        // check if downloads are ok (hashmac is good for each file).
        bool checkDownloadsAreOk() const;
        bool installLoader();
        bool installUpdater();
        bool installJava();
        bool installApp();
        bool installCnlpFile(const QString &_file);

        static QMultiMap<Application, QString> getFilesNonAlreadyInTempDir(const QMultiMap<Application, QString> _fullDownloadMap,
                                                                           const QMultiMap<Application, Download> _cnlpParsedFiles);

    signals:
        void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void downloadSpeedUpdated(const QString &_speed);
        void remainingTimeUpdated(const QString &_time);
        void serverUrlUpdated(const QUrl &_url);
        void downloadingFileUpdated(const QString &_file);
        void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void errorOccurred(const QString &_message);
};

#endif
