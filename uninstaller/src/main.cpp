#include <QCoreApplication>
#include <QDebug>
#include <QDir>

#include "log/logger.h"
#include "utils.h"

int main(int argc, char * argv[])
{
    // TODO set path installation? Must be init before installation to log
    // logger initialization
    new Logger("uninstaller.log");

    AppPath appPath = Utils::getAppPath();
    qDebug() << "root path:" << appPath.getInstallationRootPath().absolutePath();

    QCoreApplication app(argc, argv);
    return app.exec();
}
