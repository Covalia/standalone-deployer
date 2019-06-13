#include <QApplication>
#include <QDir>
#include <QTemporaryFile>

#include "log/logger.h"
#include "manager/installmanager.h"

#ifdef Q_OS_WIN
#include "manager/resources/windowsresources.h"
#endif

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    new Logger(Utils::getInstallerlLogPath());
    L_INFO(QString("Installer started: %1").arg(Utils::getAppPath().getInstallerVersion()));

    // load settings resource static file. must be called keymanager_resources
    Q_INIT_RESOURCE(keymanager_resources);

    QPalette p(qApp->palette());
    p.setColor(QPalette::Mid, p.color(QPalette::Base));
    qApp->setPalette(p);

    QTemporaryFile projectIniFile;
    if (projectIniFile.open()) {

#ifdef Q_OS_WIN
        WindowsResources::extractStyleCssToTempFile(projectIniFile.fileName());
#endif
//#ifdef Q_OS_MACOS
//#endif

        QFile file(projectIniFile.fileName());
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QString::fromLatin1(file.readAll());
            file.close();
            qApp->setStyleSheet(styleSheet);
            L_INFO("Stylesheet loaded");
        } else {
            L_ERROR("Unable to load stylesheet");
        }
    }

    L_INFO(QString("Exe path: %1").arg(QDir("./").absolutePath()));

    InstallManager installManager;
    installManager.initInstallation();

    return app.exec();
}
