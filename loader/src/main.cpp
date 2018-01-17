#include <QCoreApplication>
#include <QObject>

#include "log/logger.h"
#include "loadermanager.h"
#include "settings/settings.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    QString dirPath = app.applicationDirPath();

    new Logger(dirPath + "/Logs/loader.log");

    L_INFO("Start Loader in " +  dirPath);

    QString settingsPath = dirPath + "/Config/standalone-deployer.ini";
    L_INFO("Start read settings in " +  settingsPath);
    Settings * settings = Settings::getInstance();
    settings->initSettings(settingsPath);
    settings->readSettings();

    LoaderManager * loaderManager = new LoaderManager();

    // close properly after treatment
    QObject::connect(loaderManager, SIGNAL(closeAppSignal()), qApp, SLOT(quit()), Qt::QueuedConnection);

    loaderManager->launchUpdater();
    loaderManager->closeAppEvent();

    int ret = app.exec();

    delete loaderManager;

    return ret;
}


