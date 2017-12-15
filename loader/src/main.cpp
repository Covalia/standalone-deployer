#include <QCoreApplication>
#include <QDebug>

#include "log/logger.h"

int main(int argc, char * argv[])
{
    // TODO set path installation? Must be init before installation to log
    // logger initialization
    new Logger("loader.log");

    QCoreApplication app(argc, argv);
    return app.exec();
}
