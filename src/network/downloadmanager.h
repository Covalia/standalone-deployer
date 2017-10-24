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

        void setUrlListToDownload(const QStringList &_urlList, bool _needProgress = false);

    signals:
        void downloadsFinished();
		void downloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void downloadSpeedMessage(const QString &_speed);
        void downloadFileMessage(const QString &_file);

        // progression totale
        void totalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

	private slots:

        void startNextHeadRequest();
        void headMetaDataChanged();
        void headErrorOccured(QNetworkReply::NetworkError);
        void currentHeadFinished();

        void startNextDownload();
        void downloadMetaDataChanged();
        void downloadErrorOccured(QNetworkReply::NetworkError);
        void currentDownloadFinished();
        void downloadReadyRead();
        void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);

	private:

        void headsFinished();

        QNetworkAccessManager m_manager;

        qint64 m_totalBytesToDownload;
        qint64 m_totalBytesDownloaded;

        QMap<QUrl, long> m_mapUrlContentLength;

        QQueue<QUrl> m_errorQueue;

        QQueue<QUrl> m_headQueue;
        QNetworkReply *m_currentHead;

		QQueue<QUrl> m_downloadQueue;
		QNetworkReply *m_currentDownload;

		QSaveFile *m_saveFile;
		QString m_currentFilename;
		QString m_temporaryDir;

        /// download time to calculate download speed.
		QTime m_downloadTime;

        /// do we need to get download progress?
        bool m_needProgress;

};

#endif
