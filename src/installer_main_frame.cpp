#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include "installer_main_frame.h"
#include "ui_installer_main_frame.h"

InstallerMainFrame::InstallerMainFrame(QWidget *_parent) :
    QMainWindow(_parent),
    ui(new Ui::InstallerMainFrame)
{
    ui->setupUi(this);

	connect(ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));

    this->setAttribute(Qt::WA_QuitOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);

}

InstallerMainFrame::~InstallerMainFrame()
{
    delete ui;
}

void InstallerMainFrame::mouse_pressed()
{
	qDebug() << "MousePressed";
}

void InstallerMainFrame::mouse_left()
{
	qDebug() << "MouseLeft";
}

void InstallerMainFrame::closeEvent(QCloseEvent *_event)
{
	qDebug() << "closeEvent";
	// sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
	if (m_alreadyClosedOnMacOs) {
		_event->accept();
	}
	else {
		int ret = QMessageBox::question(this, tr("Attention"),
				tr("Vous êtes sur le point de quitter l'application, voulez vous continuer ?"),
				QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (ret == QMessageBox::Yes)
		{
			m_alreadyClosedOnMacOs = true;
			_event->accept();
		}
		else
		{
			_event->ignore();
		}
	}

}
