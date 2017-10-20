#include <QApplication>
#include <QDebug>
#include <QString>

#include "mainwindow.h"
#include "log/simpleQtLogger.h"
#include "log/logger.h"

int main(int argc, char *argv[])
{
    // logger initialization
    Logger *log = new Logger(QString("C:\\Users\\Alexis\\Desktop\\test logs\\logDeploiment.log"));

    L_INFO("Start Application");
	QApplication a(argc, argv);

	MainWindow w;
	w.show();
	w.center();


	return a.exec();
}
