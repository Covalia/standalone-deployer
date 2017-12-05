#include <QApplication>
#include <QDebug>

int main(int argc, char * argv[])
{
	qDebug() << "installer started";
    QApplication app(argc, argv);
    return app.exec();
}
