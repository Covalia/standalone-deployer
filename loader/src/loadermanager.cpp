#include "loadermanager.h"

#include <QCoreApplication>
#include <QProcess>
#include <QStringList>


#include "log/logger.h"
#include "settings/settings.h"
#include "fs/resourcesutil.h"

LoaderManager::LoaderManager() : QObject()
{
}

LoaderManager::~LoaderManager()
{
}

bool LoaderManager::launchUpdater()
{
    Settings * settings = Settings::getInstance();
    L_INFO("Install location = " + settings->getInstallLocation());

    QProcess process;
    QString updaterFile = ResourcesUtil::getUpdaterPath(settings->getInstallLocation());

    if (!QFile::exists(updaterFile)) {
        L_ERROR("An error occured when launch Updater file " + updaterFile + ". The file doesn't exist.");
        return false;
    }
    QStringList args = qApp->arguments();
    L_INFO("Launch file " + updaterFile + " with args " + args.join(""));
    bool success = process.startDetached(updaterFile, args);
    if (!success) {
        L_ERROR("Error when launching file " + updaterFile);
    } else {
        L_INFO("Success launching file " + updaterFile);
    }
    return success;
}

void LoaderManager::closeAppEvent(){
    L_INFO("Signal close app");
    closeAppSignal();
}


