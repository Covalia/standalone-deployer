#include <QApplication>
#include <QDir>
#include <cstdlib>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "log/logger.h"
#include "manager/installmanager.h"
#include "installerfactories/osresources/osresources.h"
#include "io/info.h"

int main(int argc, char * argv[])
{
#ifdef Q_OS_WIN
        DWORD procIDs[2];
        DWORD maxCount = 2;
        // get a list of the processes attached to the current console
        DWORD result = GetConsoleProcessList((LPDWORD)procIDs, maxCount);
        if (result == 1) {
            // only one process: windows application was mouse clicked
            // so, we hide the console window. this can cause artifacts when console hides..
            ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
        }
        // else, more than one process (cannot be zero), this is run from the terminal
        // so, we do not hide the console window.
        // allow to print help and installation progress, for example.
#endif

    QApplication app(argc, argv);

    QStringList arguments = qApp->arguments();
    if (arguments.contains("--version")) {
        // print to stdout on macOS and Windows, then quit.
        // we only do this way in the installer because loader / updater will
        // show black console artifact at each run... too annoying.
        const QString versionLine = QString("Version: %1\n").arg(STANDALONE_DEPLOYER_VERSION);
        const QString buildHashLine = QString("Build hash: %1\n").arg(Info::getBuildHash());
        QTextStream(stdout) << versionLine;
        QTextStream(stdout) << buildHashLine;
        app.quit();
        return 0;
    }

    new Logger(Utils::getInstallerlLogPath());

    L_INFO(QString("Installer started: %1").arg(STANDALONE_DEPLOYER_VERSION));

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
