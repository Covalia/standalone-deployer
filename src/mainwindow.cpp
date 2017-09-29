#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QTimer>
#include <QDirIterator>
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "downloadmanager.h"

MainWindow::MainWindow(QWidget *_parent) :
	QMainWindow(_parent),
	m_timer(0),
	m_ui(new Ui::MainWindow),
	m_downloader(0)
{
	m_ui->setupUi(this);

	m_downloader = new DownloadManager(this);

	connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
	connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

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
	delete m_downloader;
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
	qDebug() << "Button clicked";

	connect(m_downloader, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgress(qint64, qint64)));
	connect(m_downloader, SIGNAL(updateDownloadMessage(QString)), this, SLOT(updateDownloadMessage(QString)));

	QStringList urls;
	urls << "http://dev.covalia.fr/lanceur_covotem.php" <<
		"http://ftp.nl.debian.org/debian/dists/stretch/main/installer-amd64/current/images/netboot/mini.iso";
	m_downloader->setUrlListToDownload(urls);
}

void MainWindow::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal) {
	m_ui->progressBar->setMaximum(_bytesTotal);
	m_ui->progressBar->setValue(_bytesReceived);
}

void MainWindow::updateDownloadMessage(QString _message) {
	m_ui->speedLabel->setText(_message);
}

