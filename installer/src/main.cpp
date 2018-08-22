#include <QApplication>
#include <QDir>

#include "log/logger.h"
#include "manager/installmanager.h"
#include "settings/settings.h"
#include "gui/forms/windowui.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    // load settings resource static file. must be called keymanager_resources
    Q_INIT_RESOURCE(keymanager_resources);

    new Logger(Utils::getInstallerlLogPath());
    L_INFO("Installer started " + Utils::getAppPath().getInstallerVersion());

    L_INFO("Exe path " + QDir("./").absolutePath());

    InstallManager installManager;
    installManager.initInstallation();

    int ret = app.exec();

    return ret;
}
