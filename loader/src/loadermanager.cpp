#include "loadermanager.h"

#include <QCoreApplication>
#include <QStringList>

#include "log/logger.h"
#include "settings/settings.h"
#include "utils.h"

LoaderManager::LoaderManager() : QObject()
{
}

LoaderManager::~LoaderManager()
{
}

bool LoaderManager::launchUpdater()
{
    Settings * settings = Settings::getInstance();

    AppPath appPath = Utils::getAppPath();

    L_INFO(QString("Install location: %1").arg(appPath.getInstallationDir().absolutePath()));

    QStringList args = qApp->arguments();
    args.removeFirst();

    if (args.contains("--debug")) {
        L_INFO(QString("Loader Arguments: %1").arg(args.join(" ")));
    }

    L_INFO(QString("Starting Updater: %1").arg(settings->getUpdaterVersion()));
    bool success = appPath.startUpdater(settings->getUpdaterVersion(), args);
    if (!success) {
        L_ERROR("Error when launching updater");
    } else {
        L_INFO("Success launching updater");
    }

    return success;
}

void LoaderManager::closeAppEvent()
{
    L_INFO("Signal close app");
    closeAppSignal();
}
