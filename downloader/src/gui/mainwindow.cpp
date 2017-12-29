#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDirIterator>
#include "ui_mainwindow.h"
#include "gui/mainwindow.h"
#include "downloader/appdownloader.h"
#include "config/global.h"

/*!
 * \class MainWindow
 * \author {MP, APi}
 * \brief This is main Frame of application.
 *
 * \date october 25, 2017
 */

/*!
 *
 * \brief Constructor
 *
 */
MainWindow::MainWindow(QWidget *_parent) :
	QMainWindow(_parent),
	m_ui(new Ui::MainWindow),
	m_appDownloader(0)
{
	m_ui->setupUi(this);

	m_appDownloader = new AppDownloader(Global::AppUrl, Global::InstallationDir, this);

	connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
	connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
    connect(m_appDownloader, SIGNAL(serverUrlMessage(const QUrl &)),
            SLOT(updateServerUrlMessage(const QUrl &)));
    connect(m_appDownloader, SIGNAL(downloadFileMessage(const QString &)),
            SLOT(updateDownloadFileMessage(const QString &)));

    connect(m_appDownloader, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(updateSingleProgress(qint64, qint64)));
    connect(m_appDownloader, SIGNAL(totalDownloadProgress(qint64, qint64)),
            SLOT(updateTotalDownloadProgress(qint64, qint64)));
    connect(m_appDownloader, SIGNAL(downloadSpeedMessage(const QString &)),
            SLOT(updateDownloadSpeedMessage(const QString &)));
    connect(m_appDownloader, SIGNAL(remainingTimeMessage(const QString &)),
            SLOT(updateRemainingTimeMessage(const QString &)));


	setAttribute(Qt::WA_QuitOnClose);
	setWindowFlags(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

}

/*!
 * \brief Main application windows destructor
 */
MainWindow::~MainWindow() {
	delete m_ui;
	delete m_appDownloader;
}

void MainWindow::closeEvent(QCloseEvent *_event) {

	qDebug() << "closeEvent";
	// sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
	if (m_alreadyClosedOnMacOs) {
		_event->accept();
	}
	else {
        //: This string refers to the exit message title.
        int ret = QMessageBox::question(this, tr("Warning!"),
                //: This string refers to the exit message.
                tr("You are about to quit the application, do you want to continue?"),
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

/*!
 * \brief MainWindow::mousePressEvent
 * \param _event
 */
void MainWindow::mousePressEvent(QMouseEvent *_event) {
	m_clickedPosition = _event->pos();
}

/*!
 * \brief MainWindow::mouseMoveEvent
 * \param _event
 */
void MainWindow::mouseMoveEvent(QMouseEvent *_event) {
	move(_event->globalPos() - m_clickedPosition);
}

void MainWindow::center() {
	QRect geometry = frameGeometry();
	QPoint center = QDesktopWidget().availableGeometry().center();
	geometry.moveCenter(center);
	move(geometry.topLeft());
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

void MainWindow::updateRemainingTimeMessage(const QString &_time) {
    if (_time.isEmpty()) {
        m_ui->remainingTimeLabel->setText("");
    }
    else {
        //: This string refers to a remaining time.
        m_ui->remainingTimeLabel->setText(tr("Estimated time: %1").arg(_time));
    }
}

void MainWindow::updateServerUrlMessage(const QUrl &_url) {
    //: This string refers to a web URL.
    m_ui->serverLabel->setText(tr("Server %1").arg(_url.toString()));
}

void MainWindow::updateDownloadFileMessage(const QString &_file) {
    //: This string refers to a downloaded file.
    m_ui->currentFileLabel->setText(tr("Downloading %1").arg(_file));
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
