#include <QApplication>
#include <QDir>

#include "log/logger.h"
#include "manager/installmanager.h"
#include "settings/settings.h"
#include "gui/forms/windowui.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    new Logger("installer.log");
    L_INFO("Installer started " + Utils::getAppPath().getInstallerVersion());

    L_INFO("Exe path " + QDir("./").absolutePath());
    InstallManager * installManager = new InstallManager();
    installManager->initInstallation();

    int ret = app.exec();

    return ret;
}
