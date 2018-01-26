#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QDirIterator>

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
    m_timer(0),
    m_appUpdater(0)
{
    m_ui->setupUi(this);

    m_ui->pushButton->setAccessibleName("pageButton");
    StyleManager::transformStyle(this);

    AppTreeManager * treeManager = new AppTreeManager(QDir(Utils::getInstallationRootPath()));
    m_ui->closeButton->setIcon(QIcon(treeManager->getImagesDirPath().absolutePath()+"/close.png"));
    m_ui->titleIconLabel->setPixmap(QPixmap(treeManager->getImagesDirPath().absolutePath()+"/logo_title.png"));

    m_appUpdater = new AppUpdater(UpdaterConfig::AppUrl, UpdaterConfig::InstallationDir, this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
    m_timer->start(5000);

    connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
    connect(m_ui->pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));

    connect(m_appUpdater, SIGNAL(serverUrlMessage(const QUrl&)),
            SLOT(updateServerUrlMessage(const QUrl&)));
    connect(m_appUpdater, SIGNAL(downloadFileMessage(const QString&)),
            SLOT(updateDownloadFileMessage(const QString&)));

    connect(m_appUpdater, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateSingleProgress(qint64,qint64)));
    connect(m_appUpdater, SIGNAL(totalDownloadProgress(qint64,qint64)),
            SLOT(updateTotalDownloadProgress(qint64,qint64)));
    connect(m_appUpdater, SIGNAL(downloadSpeedMessage(const QString&)),
            SLOT(updateDownloadSpeedMessage(const QString&)));
    connect(m_appUpdater, SIGNAL(remainingTimeMessage(const QString&)),
            SLOT(updateRemainingTimeMessage(const QString&)));

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
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

void MainWindow::showEvent(QShowEvent * _event)
{
    QMainWindow::showEvent(_event);
    updateSlideShow();
}


void MainWindow::closeEvent(QCloseEvent * _event)
{
    qDebug() << "closeEvent";
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this, tr("Do you want to exit the application?"), tr("The installation will be stopped"));
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

void MainWindow::buttonClicked()
{
    m_appUpdater->start();
}

void MainWindow::updateSingleProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    m_ui->singleProgressBar->setMaximum(_bytesTotal);
    m_ui->singleProgressBar->setValue(_bytesReceived);
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
    m_ui->currentFileLabel->setText(tr("Downloading %1").arg(_file));
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
    AppTreeManager * treeManager = new AppTreeManager(QDir(Utils::getInstallationRootPath()));
    QDirIterator it(treeManager->getSlidesDirPath(), QDirIterator::Subdirectories);
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

        QString bSlideStyle = " QPushButton{                      \
                                    border-radius: 2px;                     \
                                    background : @color-text-gray;          \
                                    margin : 3px;                           \
                                }                                           \
                                QPushButton:hover {                         \
                                    background : @color-text-on-background; \
                                }                                           \
                                QPushButton:checked {                       \
                                  background : @color-text-on-background;   \
                                }";
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
        QPixmap pixmap = m_imagesList.at(_index);
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
