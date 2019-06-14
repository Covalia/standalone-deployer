#include <QApplication>
#include <QDir>
#include <cstdlib>

#include "log/logger.h"
#include "manager/installmanager.h"

#ifdef Q_OS_WIN
#include <QTemporaryFile>
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

#ifdef Q_OS_WIN
    // extract style.css to temp file
    QTemporaryFile styleCssFile;
    if (styleCssFile.open()) {
        WindowsResources::extractStyleCssToTempFile(styleCssFile.fileName());
    }
    else {
        L_ERROR("Unable to open temporary file for style.css.");
        qApp->quit();
        return EXIT_FAILURE;
    }
#endif
#ifdef Q_OS_MACOS
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cdUp();
    dir.cd("Resources");
    const QString styleCssFilePath = dir.absoluteFilePath("style.css");

    QFile styleCssFile(styleCssFilePath);
    if (styleCssFile.exists()) {
        styleCssFile.close();
    }
    else {
        L_ERROR("Unable to open style.css file from app resources.");
        styleCssFile.close();
        qApp->quit();
        return EXIT_FAILURE;
    }
#endif

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
