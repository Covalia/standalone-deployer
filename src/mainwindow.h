#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QPixmap>

class QCloseEvent;
class QMouseEvent;
class QTimer;
class QWidget;

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

	private:
		QTimer *m_timer;
		QList<QPixmap> m_imagesList;
		QPoint m_clickedPosition;
		Ui::MainWindow *m_ui;
		bool m_alreadyClosedOnMacOs = false;

		virtual void closeEvent(QCloseEvent *);
		void loadSlideShowImagesFromResources();
};

#endif
