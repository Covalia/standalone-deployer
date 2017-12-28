#ifndef INSTALLER__DOWNLOAD_H
#define INSTALLER__DOWNLOAD_H

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
        explicit DownloadUI(QWidget * _parent = 0);
        virtual ~DownloadUI();
        void setTextGlobalDownload(QString _text);
        void setTextParticularDownload(QString _text);
        void setTextPercentDownload(int _value);
        void setPercentParticularDownload(int _value);

    private:
        Ui::DownloadUI * m_ui;
        QList<QPixmap> m_imagesList;
        QList<QPushButton *> m_buttonsList;
        QTimer * m_timer;
        void showEvent(QShowEvent * _event);
        void loadSlideShowImagesFromResources();

    public slots:
        void changeLanguage();
        void updateSlideShow();
        void updateSlideShow(int _index);
        void buttonSlideEvent();
};

#endif // DOWNLOAD_H
