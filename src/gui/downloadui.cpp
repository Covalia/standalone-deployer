#include "downloadui.h"
#include "ui_downloadui.h"
#include "src/style/stylemanager.h"

#include <QString>
#include <QDirIterator>

DownloadUI::DownloadUI(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::DownloadUI){
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlideShow()));
    m_timer->start(5000);

    updateSlideShow();
}

DownloadUI::~DownloadUI(){
    delete ui;
    delete m_timer;
}

void DownloadUI::changeLanguage(){
    ui->retranslateUi(this);
}

void DownloadUI::loadSlideShowImagesFromResources(){
    const QSize maxSize = ui->labelImage->size();

    m_imagesList.clear();
    QDirIterator it(":/slideshow", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString resourcePath = it.next();
        const QPixmap pixmap = QPixmap(resourcePath);
        QPixmap resizedPixmap = pixmap.scaled(maxSize, Qt::KeepAspectRatio);
        m_imagesList << resizedPixmap;
    }
}

void DownloadUI::updateSlideShow(){
    if (m_imagesList.isEmpty()) {
        loadSlideShowImagesFromResources();
    }
    if (!m_imagesList.isEmpty()) {
        static int update_counter = -1;
        update_counter++;
        update_counter %= m_imagesList.size();
        ui->labelImage->setPixmap(m_imagesList.at(update_counter));
    }
}
