#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QFile>
#include <QObject>
#include <QQueue>
#include <QTime>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class QSaveFile;

class DownloadManager: public QObject
{

	Q_OBJECT

	public:
		explicit DownloadManager(const QString &_saveDir, QObject *parent = 0);
		virtual ~DownloadManager();

		void setUrlListToDownload(const QStringList &urlList);

	signals:
		void finished();
		void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void downloadSpeedMessage(const QString &_message);

	private slots:
		void startNextDownload();
		void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
		void downloadFinished();
		void downloadReadyRead();
		void errorOccured(QNetworkReply::NetworkError);
		void metaDataChanged();

	private:
		int m_currentDownloaderCount;
		int m_currentFileCount;

		QNetworkAccessManager m_manager;
		QQueue<QUrl> m_downloadQueue;
		QNetworkReply *m_currentDownload;
		QSaveFile *m_saveFile;
		QString m_currentFilename;
		QString m_temporaryDir;
		QTime m_downloadTime;

};

#endif
