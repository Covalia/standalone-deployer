#include "gui/downloadui.h"
#include "ui_downloadui.h"
#include "style/stylemanager.h"
//#include "log/logger.h"

#include <QString>
#include <QDirIterator>

static int update_counter = -1;

DownloadUI::DownloadUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::DownloadUI),
    m_timer(0)
{
    m_ui->setupUi(this);

    StyleManager::transformStyle(this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
    m_timer->start(5000);
}

DownloadUI::~DownloadUI()
{
    delete m_ui;
    delete m_timer;
}

void DownloadUI::showEvent(QShowEvent * _event)
{
    QWidget::showEvent(_event);
    updateSlideShow();
}

void DownloadUI::changeLanguage()
{
    m_ui->retranslateUi(this);
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
        m_ui->horizontalLayoutButtonSlider->addWidget(button);
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

void DownloadUI::updateSlideShow(int _index)
{
    if (!m_imagesList.isEmpty()) {
        const QSize maxSize = m_ui->labelImage->size();
        QPixmap pixmap = m_imagesList.at(_index);
        QPixmap resizedPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_ui->labelImage->setPixmap(resizedPixmap);
        m_buttonsList.at(_index)->setChecked(true);
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

void DownloadUI::setTextGlobalDownload(QString _text)
{
    m_ui->labelProgresseBarGlobal->setText(_text);
}

void DownloadUI::setTextParticularDownload(QString _text)
{
    m_ui->labelProgressBarFile->setText(_text);
}

void DownloadUI::setTextPercentDownload(int _value)
{
    m_ui->progressBarGlobal->setValue(_value);
}

void DownloadUI::setPercentParticularDownload(int _value)
{
    m_ui->progressBarFile->setValue(_value);
}
