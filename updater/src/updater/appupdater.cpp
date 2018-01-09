#include "updater/appupdater.h"
#include "network/downloadmanager.h"
#include "updater/config.h"
#include <QDebug>

AppUpdater::AppUpdater(const QUrl &_appUrl, const QDir &_appInstallDir, QObject *_parent) : QObject(_parent),
	m_updater(0),
	m_appTreeManager(0)
{
	m_appTreeManager = new AppTreeManager(_appInstallDir, this);
    m_appUrl = _appUrl;

    QNetworkProxy proxy;

    // TODO récupérer ici la configuration du proxy.

    m_updater = new DownloadManager(m_appTreeManager->getTempDirPath(), _appUrl, proxy, this);

    connect(m_updater, SIGNAL(downloadProgress(qint64, qint64)),
        SLOT(updateProgress(qint64, qint64)));
    connect(m_updater, SIGNAL(downloadSpeedMessage(const QString &)),
        SLOT(updateDownloadSpeedMessage(const QString &)));
    connect(m_updater, SIGNAL(remainingTimeMessage(const QString &)),
        SLOT(updateRemainingTimeMessage(const QString &)));
    connect(m_updater, SIGNAL(downloadFileMessage(const QString &)),
        SLOT(updateDownloadFileMessage(const QString &)));
    connect(m_updater, SIGNAL(totalDownloadProgress(qint64, qint64)),
        SLOT(updateTotalDownloadProgress(qint64, qint64)));
    connect(m_updater, SIGNAL(downloadsFinished()),
            SLOT(downloadFinished()));
}

AppUpdater::~AppUpdater()
{
	delete m_updater;
	delete m_appTreeManager;
}

void AppUpdater::start()
{
	qDebug() << "start app updater";
	emit serverUrlMessage(m_appUrl);

	bool result = m_appTreeManager->makeAppDirectories();
	qDebug() << "makeAppDirectories" << result;

    QList<QUrl> urls;

    urls << QUrl("lib/covotem-macosx-native.jar");
    urls << QUrl("lib/covotemLoader.jar");
    urls << QUrl("lib/covotem.jar");
    urls << QUrl("lib/external/annotations.jar");
    urls << QUrl("lib/external/balloontip-1.2.4.1.jar");


    // urls << m_appUrl + Updater::AppCnlpRemoteFilename;

    // TODO transmettre le nom de l'application avec les url des fichiers, il s'agit du sous dossier
    m_updater->setUrlListToDownload("Application", urls);
}

void AppUpdater::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
	emit downloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::updateDownloadSpeedMessage(const QString &_speed)
{
    emit downloadSpeedMessage(_speed);
}

void AppUpdater::updateRemainingTimeMessage(const QString &_time)
{
    emit remainingTimeMessage(_time);
}

void AppUpdater::updateDownloadFileMessage(const QString &_file) {
    emit downloadFileMessage(_file);
}

void AppUpdater::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal) {
    emit totalDownloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::downloadFinished() {
    qDebug() << "TODO read the cnlp";
    // TODO read xml.

}
