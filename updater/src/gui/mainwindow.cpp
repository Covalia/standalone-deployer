#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDirIterator>
#include <QTimer>
#include <QMessageBox>

#include "gui/askpopupui.h"
#include "ui_mainwindow.h"
#include "gui/mainwindow.h"
#include "updater/appupdater.h"
#include "settings/settings.h"
#include "updater/config.h"
#include "gui/style/stylemanager.h"
#include "utils.h"

/*!
 * \class MainWindow
 * \author {MP, APi}
 * \brief This is main Frame of application.
 *
 * \date october 25, 2017
 */

static int update_counter = -1;

/*!
 *
 * \brief Constructor
 *
 */
MainWindow::MainWindow(QWidget * _parent) :
    QMainWindow(_parent),
    m_ui(new Ui::MainWindow),
    m_timer(nullptr),
    m_appUpdater(nullptr)
{
    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    m_ui->setupUi(this);

    StyleManager::transformStyle(this);

    AppPath appPath = Utils::getAppPath();
    m_ui->closeButton->setIcon(QIcon(":/images/close.png"));
    m_ui->titleIconLabel->setPixmap(QPixmap(appPath.getImagesDir().absoluteFilePath("title.png")));

    QTimer::singleShot(0, this, SLOT(updateSlideShow()));

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
    m_timer->start(5000);

    connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));

    Settings * settings = Settings::getInstance();
    m_appUpdater = new AppUpdater(settings->getDeploymentUrl(), this);

    connect(m_appUpdater, SIGNAL(serverUrlUpdated(const QUrl&)),
            SLOT(updateServerUrlMessage(const QUrl&)));
    connect(m_appUpdater, SIGNAL(downloadingFileUpdated(const QString&)),
            SLOT(updateDownloadFileMessage(const QString&)));
    connect(m_appUpdater, SIGNAL(totalDownloadProgress(qint64,qint64)),
            SLOT(updateTotalDownloadProgress(qint64,qint64)));
    connect(m_appUpdater, SIGNAL(downloadSpeedUpdated(const QString&)),
            SLOT(updateDownloadSpeedMessage(const QString&)));
    connect(m_appUpdater, SIGNAL(remainingTimeUpdated(const QString&)),
            SLOT(updateRemainingTimeMessage(const QString&)));
    connect(m_appUpdater, SIGNAL(errorOccurred(const QString&)),
            SLOT(handleDownloaderError(const QString&)));

    QTimer::singleShot(0, this, SLOT(startUpdate()));
}

/*!
 * \brief Main application windows destructor
 */
MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_appUpdater;
    delete m_timer;
}

void MainWindow::closeEvent(QCloseEvent * _event)
{
    qDebug() << "closeEvent";
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this);
        popupClose->show();
        if (popupClose->exec() == QDialog::Accepted) {
            _event->accept();
            m_alreadyClosedOnMacOs = true;
        } else {
            _event->ignore();
        }
    }
}

/*!
 * \brief MainWindow::mousePressEvent
 * \param _event
 */
void MainWindow::mousePressEvent(QMouseEvent * _event)
{
    m_clickedPosition = _event->pos();
}

/*!
 * \brief MainWindow::mouseMoveEvent
 * \param _event
 */
void MainWindow::mouseMoveEvent(QMouseEvent * _event)
{
    move(_event->globalPos() - m_clickedPosition);
}

void MainWindow::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void MainWindow::startUpdate()
{
    m_appUpdater->start();
}

void MainWindow::updateDownloadSpeedMessage(const QString &_speed)
{
    m_ui->speedLabel->setText(_speed);
}

void MainWindow::updateRemainingTimeMessage(const QString &_time)
{
    if (_time.isEmpty()) {
        m_ui->remainingTimeLabel->setText("");
    } else {
        //: This string refers to a remaining time.
        m_ui->remainingTimeLabel->setText(tr("Estimated time: %1").arg(_time));
    }
}

void MainWindow::updateServerUrlMessage(const QUrl &_url)
{
    //: This string refers to a web URL.
    m_ui->serverLabel->setText(tr("Server %1").arg(_url.toString()));
}

void MainWindow::updateDownloadFileMessage(const QString &_file)
{
    //: This string refers to a downloaded file.
    if (_file.isEmpty()) {
        m_ui->currentFileLabel->setText("");
    } else {
        m_ui->currentFileLabel->setText(tr("Downloading %1").arg(_file));
    }
}

void MainWindow::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    if (_bytesReceived == 0 && _bytesTotal == 0) {
        // affichage d'une progressbar indéfinie
        m_ui->totalProgressBar->setMinimum(0);
        m_ui->totalProgressBar->setMaximum(0);
        m_ui->totalProgressBar->setValue(0);
    } else if (_bytesTotal > 0) {
        m_ui->totalProgressBar->setMaximum(100);
        m_ui->totalProgressBar->setValue(static_cast<int>(_bytesReceived * 100.0 / _bytesTotal));
    }
}

void MainWindow::loadSlideShowImagesFromResources()
{
    m_imagesList.clear();
    AppPath appPath = Utils::getAppPath();
    QDirIterator it(appPath.getSlidesDir(), QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString resourcePath = it.next();
        const QPixmap pixmap = QPixmap(resourcePath);
        if (!pixmap.isNull()) {
            m_imagesList << pixmap;
        }
    }
    std::random_shuffle(m_imagesList.begin(), m_imagesList.end());

    QButtonGroup * group = new QButtonGroup(this);
    group->setExclusive(true);
    m_buttonsList.clear();
    for (int i = 0; i < m_imagesList.size(); i++) {
        QPushButton * button = new QPushButton("", this);
        button->setMaximumSize(QSize(10, 10));
        button->setCheckable(true);

        QString bSlideStyle = "QPushButton {"
            "border-radius:2px;"
            "background:@gray-text-color;"
            "margin:3px;"
            "}"
            "QPushButton:hover {"
            "background:@default-text-color;"
            "}"
            "QPushButton:checked {"
            "background:@default-text-color;"
            "}";
        button->setStyleSheet(StyleManager::transformStyle(bSlideStyle));
        button->setCursor(Qt::PointingHandCursor);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonSlideEvent()));
        m_ui->horizontalLayoutButtonSlider->addWidget(button);
        m_buttonsList << button;
        group->addButton(button);
    }
}

void MainWindow::buttonSlideEvent()
{
    QPushButton * button = qobject_cast<QPushButton *>(sender());

    if (button) {
        m_timer->stop();
        m_timer->start();
        int index = m_buttonsList.indexOf(button);
        update_counter = index;
        updateSlideShow(index);
    }
}

void MainWindow::updateSlideShow(int _index)
{
    if (!m_imagesList.isEmpty()) {
        const QSize maxSize = m_ui->labelImage->size();
        const QPixmap &pixmap = m_imagesList.at(_index);
        QPixmap resizedPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_ui->labelImage->setPixmap(resizedPixmap);
        m_buttonsList.at(_index)->setChecked(true);
    }
}

void MainWindow::updateSlideShow()
{
    if (m_imagesList.isEmpty()) {
        loadSlideShowImagesFromResources();
    }

    update_counter++;
    update_counter %= m_imagesList.size();
    updateSlideShow(update_counter);
}

void MainWindow::handleDownloaderError(const QString &_message)
{
    m_ui->totalProgressBar->setVisible(false);
    m_ui->currentFileLabel->setVisible(false);
    m_ui->remainingTimeLabel->setVisible(false);
    m_ui->speedLabel->setVisible(false);

    QMessageBox msgBox(QMessageBox::Critical, tr("An error occurred!"),
                       QString("%1\n\n%2").arg(tr("The application will quit.")).arg(_message),
                       QMessageBox::Ok, this, Qt::Window | Qt::FramelessWindowHint);
    msgBox.exec();

    qApp->quit();
}
