#include <QApplication>
#include <QDebug>
#include "installer_main_frame.h"

int main(int argc, char *argv[])
{
	qDebug() << "Starting application";
    QApplication a(argc, argv);
    InstallerMainFrame w;
    w.show();

    return a.exec();
}
