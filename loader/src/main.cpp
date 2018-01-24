#include <QCoreApplication>
#include <QObject>

#include "log/logger.h"
#include "loadermanager.h"
#include "settings/settings.h"
#include "fs/apptreemanager.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    QString dirPath = QDir::currentPath();

    AppTreeManager * treeManager = new AppTreeManager(QDir(dirPath));

    new Logger(treeManager->getLogsDirPath().absolutePath() + "/loader.log");
    L_INFO("Start Loader in " +  dirPath);

    QString settingsPath = treeManager->getConfigurationFilePath();
    L_INFO("Start read settings in " +  settingsPath);

    Settings * settings = Settings::getInstance();
    settings->initSettings(settingsPath);
    settings->readSettings();
    L_INFO("---------Settings info ------------");
    L_INFO(settings->paramListString());
    L_INFO("-----------------------------------");

    LoaderManager * loaderManager = new LoaderManager();

    // close properly after treatment
    QObject::connect(loaderManager, SIGNAL(closeAppSignal()), qApp, SLOT(quit()), Qt::QueuedConnection);

    loaderManager->launchUpdater();
    loaderManager->closeAppEvent();

    return app.exec();
}
