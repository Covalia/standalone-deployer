#include <QCoreApplication>
#include <QDebug>

int main(int argc, char * argv[])
{
	qDebug() << "loader started";
    QCoreApplication app(argc, argv);
    return app.exec();
}
