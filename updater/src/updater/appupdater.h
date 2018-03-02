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

    static QMap<Application, QList<QUrl> > downloadsToUrls(const QMap<Application, QList<Download> > &_downloadsMap);

private:
    DownloadManager * m_updater;
    QUrl m_appUrl;
    AppPath m_appPath;

signals:
    void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
    void downloadSpeedMessage(const QString &_speed);
    void remainingTimeMessage(const QString &_time);
    void serverUrlMessage(const QUrl &_url);
    void downloadFileMessage(const QString &_file);
    void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
};

#endif // ifndef UPDATER__APPUPDATER_H
