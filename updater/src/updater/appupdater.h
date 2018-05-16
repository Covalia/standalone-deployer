#ifndef UPDATER__APPUPDATER_H
#define UPDATER__APPUPDATER_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QList>
#include <QMap>
#include "xml/data/application.h"
#include "xml/data/download.h"
#include "utils.h"

class DownloadManager;

class AppUpdater : public QObject
{
    Q_OBJECT

public:
    explicit AppUpdater(const QUrl &_appUrl, const QDir &_appInstallDir, QObject * _parent = 0);
    virtual ~AppUpdater();
    void start();

private slots:
    void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);
    void updateDownloadSpeedMessage(const QString &_speed);
    void updateRemainingTimeMessage(const QString &_time);
    void updateDownloadFileMessage(const QString &_file);
    void updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

    void cnlpDownloadFinished();
    void applicationDownloadFinished();

    QList<QString> getLocalFiles(const Application &_application);
    QMap<Application, QList<QString> > getLocalFiles();
    void processCnlpDownloadFileList();
    bool buildApplicationInTempDirectory(const Application &_application);

    // return true if the application has changed
    bool doesAppNeedToBeRebuild(const Application &_application);


private:
    DownloadManager * m_updater;
    QUrl m_appUrl;
    AppPath m_appPath;

    QString m_remoteUpdaterVersion;
    QString m_localUpdaterVersion;
    QString m_remoteJavaVersion;
    QString m_localJavaVersion;

    // list of remote files to download
    QMap<Application, QList<Download> > m_cnlpParsedFiles;
    // list of application files to download, those who have changed
    QMap<Application, QList<QString> > m_filesToDownload;
    // list of application files to keep, those who haven't changed
    QMap<Application, QList<QString> > m_filesToKeep;
    // list of remaining application files, those who need to be removed
    QMap<Application, QList<QString> > m_remainingFiles;

    // encoding to use to start application
    QString m_encoding;
    // main class to start
    QString m_mainClass;

    // received arguments list from cnlp
    QStringList m_arguments;

    // memory to set when starting application
    QString m_memory;

signals:
    void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
    void downloadSpeedMessage(const QString &_speed);
    void remainingTimeMessage(const QString &_time);
    void serverUrlMessage(const QUrl &_url);
    void downloadFileMessage(const QString &_file);
    void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
};

#endif // ifndef UPDATER__APPUPDATER_H
