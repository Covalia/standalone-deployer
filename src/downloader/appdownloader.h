#ifndef APP_DOWNLOADER_H
#define APP_DOWNLOADER_H

#include <QObject>
#include "fs/apptreemanager.h"

class DownloadManager;

class AppDownloader : public QObject
{

	Q_OBJECT

	public:
		explicit AppDownloader(QString _appUrl, QString _appInstallDir, QObject *_parent = 0);
		virtual ~AppDownloader();
		void start();

	private slots:
		void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);
		void updateDownloadSpeedMessage(QString _message);

	private:
		DownloadManager *m_downloader;
		QString m_appUrl;
		AppTreeManager *m_appTreeManager;

	signals:
		void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
		void downloadSpeedMessage(QString _message);

};

#endif
