#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPixmap>

class QCloseEvent;
class QMouseEvent;
class QTimer;
class QWidget;
class DownloadManager;

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
		void updateDownloadMessage(QString _message);

	private:
		QTimer *m_timer;
		QList<QPixmap> m_imagesList;
		QPoint m_clickedPosition;
		Ui::MainWindow *m_ui;
		bool m_alreadyClosedOnMacOs = false;
		DownloadManager *m_downloader;

		virtual void closeEvent(QCloseEvent *);
		void loadSlideShowImagesFromResources();
};

#endif
