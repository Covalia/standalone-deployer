#include <QApplication>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	qDebug() << "Starting application";
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	w.center();

	return a.exec();
}
