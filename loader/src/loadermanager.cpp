#include "loadermanager.h"

#include <QCoreApplication>
#include <QProcess>
#include <QStringList>

#include "log/logger.h"
#include "settings/settings.h"
#include "fs/apptreemanager.h"

LoaderManager::LoaderManager() : QObject()
{
}

LoaderManager::~LoaderManager()
{
}

bool LoaderManager::launchUpdater()
{
    Settings * settings = Settings::getInstance();

    QString installLocation = settings->getInstallLocation();

    L_INFO("Install location = " + installLocation);

    AppTreeManager * treeManager = new AppTreeManager(QDir(installLocation));

    QProcess process;
    QString updaterFile = treeManager->getUpdaterFilePath(settings->getUpdaterVersion());

    if (!QFile::exists(updaterFile)) {
        L_ERROR("An error occured when launch Updater file " + updaterFile + ". The file doesn't exist.");
        return false;
    }
    QStringList args = qApp->arguments();
    args.removeFirst();
    L_INFO("Launch file " + updaterFile + " with args " + args.join(""));

    QString sOldPath = QDir::currentPath();
    QDir::setCurrent(installLocation);

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
