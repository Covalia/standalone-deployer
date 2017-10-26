#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QTimer>
#include <QDirIterator>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "downloader/appdownloader.h"
#include "config/global.h"

MainWindow::MainWindow(QWidget *_parent) :
	QMainWindow(_parent),
	m_timer(0),
	m_ui(new Ui::MainWindow),
	m_appDownloader(0)
{
	m_ui->setupUi(this);

	m_appDownloader = new AppDownloader(Global::AppUrl, Global::InstallationDir, this);

	connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
	connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_appDownloader, SIGNAL(serverUrlMessage(const QString &)),
            SLOT(updateServerUrlMessage(const QString &)));
    connect(m_appDownloader, SIGNAL(downloadFileMessage(const QString &)),
            SLOT(updateDownloadFileMessage(const QString &)));

    connect(m_appDownloader, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(updateSingleProgress(qint64, qint64)));
    connect(m_appDownloader, SIGNAL(totalDownloadProgress(qint64, qint64)),
            SLOT(updateTotalDownloadProgress(qint64, qint64)));
    connect(m_appDownloader, SIGNAL(downloadSpeedMessage(const QString &)),
            SLOT(updateDownloadSpeedMessage(const QString &)));

	setAttribute(Qt::WA_QuitOnClose);
	setWindowFlags(Qt::FramelessWindowHint);

	m_ui->descriptionLabel->setText(tr("Bienvenue dans l'installeur de Covotem !"));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
	m_timer->start(5000);

	updateSlideShow();

}

MainWindow::~MainWindow() {
	delete m_ui;
	delete m_timer;
	delete m_appDownloader;
}

void MainWindow::closeEvent(QCloseEvent *_event) {

	qDebug() << "closeEvent";
	// sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
	if (m_alreadyClosedOnMacOs) {
		_event->accept();
	}
	else {
		int ret = QMessageBox::question(this, tr("Attention"),
				tr("Vous êtes sur le point de quitter l'application, voulez vous continuer ?"),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (ret == QMessageBox::Yes)
		{
			m_alreadyClosedOnMacOs = true;
			_event->accept();
		}
		else
		{
			_event->ignore();
		}
	}

}

void MainWindow::mousePressEvent(QMouseEvent *_event) {
	m_clickedPosition = _event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *_event) {
	move(_event->globalPos() - m_clickedPosition);
}

void MainWindow::center() {
	QRect geometry = frameGeometry();
	QPoint center = QDesktopWidget().availableGeometry().center();
	geometry.moveCenter(center);
	move(geometry.topLeft());
}

void MainWindow::updateSlideShow() {

	if (m_imagesList.isEmpty()) {
		loadSlideShowImagesFromResources();
	}

	if (!m_imagesList.isEmpty()) {
		static int update_counter = -1;
		update_counter++;
		update_counter %= m_imagesList.size();

		m_ui->imageLabel->setPixmap(m_imagesList.at(update_counter));
	}

}

void MainWindow::loadSlideShowImagesFromResources() {
	const QSize maxSize = m_ui->imageLabel->maximumSize();
	qDebug() << maxSize;

	m_imagesList.clear();
	QDirIterator it(":/slideshow", QDirIterator::Subdirectories);
	while (it.hasNext()) {
		const QString resourcePath = it.next();
		const QPixmap pixmap = QPixmap(resourcePath);
		qDebug() << pixmap.width() << "x" << pixmap.height();
		QPixmap resizedPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio);
		m_imagesList << resizedPixmap;
	}

}

void MainWindow::buttonClicked() {
	m_appDownloader->start();
}

void MainWindow::updateSingleProgress(qint64 _bytesReceived, qint64 _bytesTotal) {
    m_ui->singleProgressBar->setMaximum(_bytesTotal);
    m_ui->singleProgressBar->setValue(_bytesReceived);
}

void MainWindow::updateDownloadSpeedMessage(const QString &_speed) {
    m_ui->speedLabel->setText(_speed);
}

void MainWindow::updateServerUrlMessage(const QString &_url) {
    m_ui->serverLabel->setText(tr("Serveur %1").arg(_url));
}

void MainWindow::updateDownloadFileMessage(const QString &_file) {
    m_ui->currentFileLabel->setText(tr("Téléchargement de %1").arg(_file));
}

void MainWindow::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal) {
    if (_bytesReceived ==0 && _bytesTotal == 0) {
        // affichage d'une progressbar indéfinie
        m_ui->totalProgressBar->setMinimum(0);
        m_ui->totalProgressBar->setMaximum(0);
        m_ui->totalProgressBar->setValue(0);
    }
    else if (_bytesTotal > 0) {
        m_ui->totalProgressBar->setMaximum(100);
        m_ui->totalProgressBar->setValue(static_cast<int>(_bytesReceived * 100.0 / _bytesTotal));
    }
}
