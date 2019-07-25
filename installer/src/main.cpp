#include <QApplication>
#include <QDir>
#include <cstdlib>

#include "log/logger.h"
#include "manager/installmanager.h"
#include "installerfactories/osresources/osresources.h"
#include "io/info.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QStringList arguments = qApp->arguments();

    if (arguments.contains("--version")) {
        // TODO console app on windows
        QTextStream(stdout) << QString("Build hash: %1\n").arg(Info::getBuildHash());
        app.quit();
        return 0;
    }

    new Logger(Utils::getInstallerlLogPath());

    L_INFO(QString("Installer started: %1").arg(Utils::getAppPath().getInstallerVersion()));

    // load settings resource static file. must be called keymanager_resources
    Q_INIT_RESOURCE(keymanager_resources);

    QPalette p(qApp->palette());
    p.setColor(QPalette::Mid, p.color(QPalette::Base));
    qApp->setPalette(p);

    // extract style.css to temp file
    QTemporaryFile styleCssFile;
    if (styleCssFile.open()) {
        if (OsResources::extractStyleCssToTempFile(styleCssFile.fileName())) {
            L_INFO("style.css extracted from application resources.");
        } else {
            L_ERROR("Unable to open style.css file from application resources.");
            qApp->quit();
            return EXIT_FAILURE;
        }
    } else {
        L_ERROR("Unable to open temporary file for style.css.");
        qApp->quit();
        return EXIT_FAILURE;
    }

    QFile file(styleCssFile.fileName());
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QString::fromLatin1(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
        L_INFO("Stylesheet loaded");
    } else {
        L_ERROR("Unable to load stylesheet");
    }

    L_INFO(QString("Exe path: %1").arg(QDir("./").absolutePath()));

    InstallManager installManager;
    installManager.initInstallation();

    return app.exec();
}
