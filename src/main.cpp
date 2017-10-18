#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "xml/deploymentxml.h"

int main(int argc, char *argv[])
{

	qDebug() << "Starting application";
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	w.center();

    //DeploymentXML parserCNLP(QString("C:\\Users\\Alexis\\Desktop\\application.cnlp"));

	return a.exec();
}
