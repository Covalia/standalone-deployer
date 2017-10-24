#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPixmap>

class QCloseEvent;
class QMouseEvent;
class QTimer;
class QWidget;
class AppDownloader;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *_parent = 0);
		~MainWindow();
		void center();

	protected:
		void mousePressEvent(QMouseEvent *);
		void mouseMoveEvent(QMouseEvent *);

	private slots:
		void updateSlideShow();
		void buttonClicked();
		void updateProgress(qint64 _bytesReceived, qint64 _bytesTotal);
        void updateDownloadSpeedMessage(const QString &_speed);
        void updateServerUrlMessage(const QString &_url);
        void updateDownloadFileMessage(const QString &_file);
        void updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal);

	private:
		QTimer *m_timer;
		QList<QPixmap> m_imagesList;
		QPoint m_clickedPosition;
		Ui::MainWindow *m_ui;
		bool m_alreadyClosedOnMacOs = false;
		AppDownloader *m_appDownloader;

		virtual void closeEvent(QCloseEvent *);
		void loadSlideShowImagesFromResources();
};

#endif
