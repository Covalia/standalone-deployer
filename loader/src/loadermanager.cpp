#include "loadermanager.h"

#include <QCoreApplication>
#include <QProcess>
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

    L_INFO("Install location = " + appPath.getInstallationRootPath().absolutePath());

    QProcess process;
    QString updaterFile = appPath.getUpdaterFilePath(settings->getUpdaterVersion());

    if (!QFile::exists(updaterFile)) {
        L_ERROR("An error occured when launch Updater file " + updaterFile + ". The file doesn't exist.");
        return false;
    }
    QStringList args = qApp->arguments();
    args.removeFirst();
    L_INFO("Launch file " + updaterFile + " with args " + args.join(""));

    QString sOldPath = QDir::currentPath();
    QDir::setCurrent(appPath.getInstallationRootPath().absolutePath());

    bool success = process.startDetached(updaterFile, args);
    if (!success) {
        L_ERROR("Error when launching file " + updaterFile);
    } else {
        L_INFO("Success launching file " + updaterFile);
    }

    QDir::setCurrent(sOldPath);

    return success;
}

void LoaderManager::closeAppEvent()
{
    L_INFO("Signal close app");
    closeAppSignal();
}
