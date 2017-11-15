#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QTimer>
#include <QDirIterator>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "style/stylemanager.h"

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
MainWindow::MainWindow(QWidget * _parent) :
    QMainWindow(_parent),
    m_timer(0),
    m_ui(new Ui::MainWindow){
    m_ui->setupUi(this);

    this->setStyleSheet(StyleManager::transformStyle(this->styleSheet()));
    this->centralWidget()->setStyleSheet(StyleManager::transformStyle(this->centralWidget()->styleSheet()));


    connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    //: This string refers to the welcome screen.
    m_ui->descriptionLabel->setText(tr("Bienvenue dans l'installeur de %1 !").arg("Covotem"));

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
	m_timer->start(5000);

    updateSlideShow();
}

/*!
 * \brief Main application windows destructor
 */
MainWindow::~MainWindow(){
    delete m_ui;
    delete m_timer;
}

void MainWindow::closeEvent(QCloseEvent *_event)
{
	qDebug() << "closeEvent";
	// sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
	if (m_alreadyClosedOnMacOs) {
		_event->accept();
	}
	else {
        //: This string refers to the exit message title.
        int ret = QMessageBox::question(this, tr("Attention !"),
                //: This string refers to the exit message.
				tr("Vous êtes sur le point de quitter l'application, voulez vous continuer ?"),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (ret == QMessageBox::Yes) {
            m_alreadyClosedOnMacOs = true;
            _event->accept();
        } else {
            _event->ignore();
        }
    }
}

/*!
 * \brief MainWindow::mousePressEvent
 * \param _event
 */
void MainWindow::mousePressEvent(QMouseEvent * _event){
    m_clickedPosition = _event->pos();
}

/*!
 * \brief MainWindow::mouseMoveEvent
 * \param _event
 */
void MainWindow::mouseMoveEvent(QMouseEvent * _event){
    move(_event->globalPos() - m_clickedPosition);
}


void MainWindow::center(){
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void MainWindow::updateSlideShow(){
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

void MainWindow::loadSlideShowImagesFromResources(){
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
