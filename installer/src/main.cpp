#include <QApplication>
#include <QDir>

#include "log/logger.h"
#include "manager/installmanager.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    // load settings resource static file. must be called keymanager_resources
    Q_INIT_RESOURCE(keymanager_resources);

    QPalette p(qApp->palette());
    p.setColor(QPalette::Mid, p.color(QPalette::Base));
    qApp->setPalette(p);

    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
	file.close();
    qApp->setStyleSheet(styleSheet);

    new Logger(Utils::getInstallerlLogPath());
    L_INFO("Installer started " + Utils::getAppPath().getInstallerVersion());

    L_INFO("Exe path " + QDir("./").absolutePath());

    InstallManager installManager;
    installManager.initInstallation();

    return app.exec();
}
