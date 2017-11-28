#include "downloadui.h"
#include "ui_downloadui.h"
#include "src/style/stylemanager.h"
#include "src/log/simpleqtlogger.h"

#include <QString>
#include <QDirIterator>
#include <algorithm>

static int update_counter = -1;

DownloadUI::DownloadUI(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::DownloadUI)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
    m_timer->start(5000);
}

DownloadUI::~DownloadUI()
{
    delete ui;
    delete m_timer;
}

void DownloadUI::showEvent(QShowEvent * event)
{
    QWidget::showEvent(event);
    updateSlideShow();
}

void DownloadUI::changeLanguage()
{
    ui->retranslateUi(this);
}

void DownloadUI::loadSlideShowImagesFromResources()
{
    m_imagesList.clear();
    QDirIterator it(":/slideshow", QDirIterator::Subdirectories);
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
        button->setStyleSheet(" QPushButton{                            \
                                    border-radius: 2px;                 \
                                    background : rgba(100,100,100, 0.3);\
                                    margin : 3px;                       \
                                }                                       \
                                QPushButton:hover {                     \
                                    background : white;                 \
                                }                                       \
                                QPushButton:checked {                   \
                                  background : white;                   \
                                }                                       \
        ");
        button->setCursor(Qt::PointingHandCursor);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonSlideEvent()));
        ui->horizontalLayoutButtonSlider->addWidget(button);
        m_buttonsList << button;
        group->addButton(button);
    }
} // DownloadUI::loadSlideShowImagesFromResources

void DownloadUI::buttonSlideEvent()
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

void DownloadUI::updateSlideShow(int index)
{
    if (!m_imagesList.isEmpty()) {
        const QSize maxSize = ui->labelImage->size();
        QPixmap pixmap = m_imagesList.at(index);
        QPixmap resizedPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labelImage->setPixmap(resizedPixmap);
        m_buttonsList.at(index)->setChecked(true);
    }
}

void DownloadUI::updateSlideShow()
{
    if (m_imagesList.isEmpty()) {
        loadSlideShowImagesFromResources();
    }
    update_counter++;
    update_counter %= m_imagesList.size();
    updateSlideShow(update_counter);
}

void DownloadUI::setTextGlobalDownload(QString text)
{
    ui->labelProgresseBarGlobal->setText(text);
}

void DownloadUI::setTextParticularDownload(QString text)
{
    ui->labelProgressBarFile->setText(text);
}

void DownloadUI::setTextPercentDownload(int value)
{
    ui->progressBarGlobal->setValue(value);
}

void DownloadUI::setPercentParticularDownload(int value)
{
    ui->progressBarFile->setValue(value);
}
