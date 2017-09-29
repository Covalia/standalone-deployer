#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkAccessManager>

class DownloadManager: public QObject
{
    Q_OBJECT

	public:
		DownloadManager(QObject *parent = 0);

		void append(const QUrl &url);
		void append(const QStringList &urlList);
		QString saveFileName(const QUrl &url);

	signals:
		void finished();
		void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
		void updateDownloadMessage(QString _message);

	private slots:
		void startNextDownload();
		void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
		void downloadFinished();
		void downloadReadyRead();

	private:
		QNetworkAccessManager manager;
		QQueue<QUrl> downloadQueue;
		QNetworkReply *currentDownload;
		QFile output;
		QTime downloadTime;

		int downloadedCount;
		int totalCount;
};

#endif
