#ifndef INSTALLER_MAIN_FRAME_H
#define INSTALLER_MAIN_FRAME_H

#include <QMainWindow>

class QCloseEvent;

namespace Ui {
	class InstallerMainFrame;
}

class InstallerMainFrame : public QMainWindow
{
    Q_OBJECT

	public:
		explicit InstallerMainFrame(QWidget *_parent = 0);
		~InstallerMainFrame();

	private slots:
		void mouse_pressed();
		void mouse_left();

	private:
		Ui::InstallerMainFrame *ui;
		bool m_alreadyClosedOnMacOs = false;

		virtual void closeEvent(QCloseEvent *);
};

#endif
