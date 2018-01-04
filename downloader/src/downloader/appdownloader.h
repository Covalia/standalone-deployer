#ifndef DOWNLOADER__APPDOWNLOADER_H
#define DOWNLOADER__APPDOWNLOADER_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include "fs/apptreemanager.h"

class DownloadManager;

class AppDownloader : public QObject
{
    Q_OBJECT

public:
    explicit AppDownloader(const QUrl &_appUrl, const QDir &_appInstallDir, QObject * _parent = 0);
    virtual ~AppDownloader();
    void start();

private slots:
    void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);
    void updateDownloadSpeedMessage(const QString &_speed);
    void updateRemainingTimeMessage(const QString &_time);
    void updateDownloadFileMessage(const QString &_file);
    void updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

    void downloadFinished();

private:
    DownloadManager * m_downloader;
    QUrl m_appUrl;
    AppTreeManager * m_appTreeManager;

signals:
    void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
    void downloadSpeedMessage(const QString &_speed);
    void remainingTimeMessage(const QString &_time);
    void serverUrlMessage(const QUrl &_url);
    void downloadFileMessage(const QString &_file);
    void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
};

#endif // ifndef DOWNLOADER__APPDOWNLOADER_H