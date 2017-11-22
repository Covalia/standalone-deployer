#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>

namespace Ui {
class DownloadUI;
}

class DownloadUI : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadUI(QWidget * parent = 0);
    ~DownloadUI();

private:
    Ui::DownloadUI * ui;
    QList<QPixmap> m_imagesList;
    QTimer * m_timer;

    void loadSlideShowImagesFromResources();

public slots:
    void changeLanguage();
    void updateSlideShow();
};

#endif // DOWNLOAD_H
