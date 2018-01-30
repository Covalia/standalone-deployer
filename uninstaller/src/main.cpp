#include <QCoreApplication>
#include <QDebug>

#include "log/logger.h"
#include "utils.h"

int main(int argc, char * argv[])
{
    // TODO set path installation? Must be init before installation to log
    // logger initialization
    new Logger("uninstaller.log");

    QString rootPath = Utils::getInstallationRootPath();
    qDebug() << "root path:" << rootPath;

    QCoreApplication app(argc, argv);
    return app.exec();
}
