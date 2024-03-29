#ifndef UPDATER_MAINWINDOW_H
#define UPDATER_MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QPixmap>
#include <QPushButton>

class QCloseEvent;
class QMouseEvent;
class QWidget;
class AppUpdater;
class QTimer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget * _parent = nullptr);
        virtual ~MainWindow();
        void center();

    protected:
        virtual void mousePressEvent(QMouseEvent *);
        virtual void mouseMoveEvent(QMouseEvent *);
        virtual void closeEvent(QCloseEvent *);

    private slots:
        void startUpdate();
        void updateDownloadSpeedMessage(const QString &_speed);
        void updateRemainingTimeMessage(const QString &_time);
        void updateServerUrlMessage(const QUrl &_url);
        void updateDownloadFileMessage(const QString &_file);
        void updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void updateServerName(const QString &_serverName);

        void updateSlideShow();
        void updateSlideShow(int _index);
        void buttonSlideEvent();
        void handleDownloaderError(const QString &_message);

    private:
        QPoint m_clickedPosition;
        Ui::MainWindow * m_ui;
        QList<QPixmap> m_imagesList;
        QList<QPushButton *> m_buttonsList;
        QTimer * m_timer;
        bool m_alreadyClosedOnMacOs = false;
        AppUpdater * m_appUpdater;

        void loadSlideShowImagesFromResources();
};

#endif
