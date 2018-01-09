#ifndef UPDATER__MAINWINDOW_H
#define UPDATER__MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>

class QCloseEvent;
class QMouseEvent;
class QWidget;
class AppUpdater;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget * _parent = 0);
        ~MainWindow();
        void center();

    protected:
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

    private slots:
        void buttonClicked();
        void updateSingleProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void updateDownloadSpeedMessage(const QString &_speed);
        void updateRemainingTimeMessage(const QString &_time);
        void updateServerUrlMessage(const QUrl &_url);
        void updateDownloadFileMessage(const QString &_file);
        void updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

    private:
        QPoint m_clickedPosition;
        Ui::MainWindow * m_ui;
        bool m_alreadyClosedOnMacOs = false;
        AppUpdater * m_appUpdater;

        virtual void closeEvent(QCloseEvent *);
};

#endif // ifndef UPDATER__MAINWINDOW_H
