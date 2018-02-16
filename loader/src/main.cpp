#include <QCoreApplication>
#include <QObject>
#include <QtDebug>

#include "log/logger.h"
#include "loadermanager.h"
#include "settings/settings.h"
#include "utils.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    AppPath appPath = Utils::getAppPath();
    qDebug() << "-- Installation root: " << appPath.getInstallationRootPath();

    new Logger(appPath.getLogsDirPath().absolutePath() + "/loader.log");

    QString settingsPath = appPath.getConfigurationFilePath();
    L_INFO("Start read settings in " +  settingsPath);

    Settings * settings = Settings::getInstance();
    settings->initSettings(settingsPath);
    settings->readSettings();
    L_INFO("---------Settings info ------------");
    L_INFO(settings->paramListString());
    L_INFO("-----------------------------------");

    LoaderManager loaderManager;

    // close properly after treatment
    QObject::connect(&loaderManager, SIGNAL(closeAppSignal()), qApp, SLOT(quit()), Qt::QueuedConnection);

    loaderManager.launchUpdater();
    loaderManager.closeAppEvent();

    return app.exec();
}
