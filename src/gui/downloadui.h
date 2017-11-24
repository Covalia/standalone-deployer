#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QPushButton>

namespace Ui {
class DownloadUI;
}

/**
 * @class DownloadUI
 * @brief Download widget
 */
class DownloadUI : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadUI(QWidget * parent = 0);
    ~DownloadUI();
    void setTextGlobalDownload(QString text);
    void setTextParticularDownload(QString text);
    void setTextPercentDownload(int value);
    void setPercentParticularDownload(int value);

private:
    Ui::DownloadUI * ui;
    QList<QPixmap> m_imagesList;
    QList<QPushButton *> m_buttonsList;
    QTimer * m_timer;
    void showEvent(QShowEvent * event);
    void loadSlideShowImagesFromResources();

public slots:
    void changeLanguage();
    void updateSlideShow();
    void updateSlideShow(int index);
    void buttonSlideEvent();
};

#endif // DOWNLOAD_H
