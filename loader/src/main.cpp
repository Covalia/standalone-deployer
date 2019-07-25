#include <QCoreApplication>
#include <QObject>
#include <QtDebug>

#include "log/logger.h"
#include "loadermanager.h"
#include "settings/settings.h"
#include "io/info.h"
#include "utils.h"

int main(int argc, char * argv[])
{
    QCoreApplication app(argc, argv);

    // load settings resource static file. must be called keymanager_resources
    Q_INIT_RESOURCE(keymanager_resources);

    AppPath appPath = Utils::getAppPath();
    qDebug() << "-- Installation root: " << appPath.getInstallationDir();

    appPath.makeAppDirectories();
    new Logger(appPath.getLogsDir().absoluteFilePath("loader.log"));

    QStringList arguments = qApp->arguments();
    if (arguments.contains("--version")) {
        const QString buildHashLine = QString("Build hash: %1\n").arg(Info::getBuildHash());
#ifdef Q_OS_MACOS
        // print on stdout on macOS
        QTextStream(stdout) << buildHashLine;
#endif
#ifdef Q_OS_WIN
        // print in log on windows. only console app can print to stdout...
        L_INFO(buildHashLine.trimmed());
#endif
        app.quit();
        return 0;
    }

    QSharedPointer<QFile> settingsPath = appPath.getConfigurationFile();
    L_INFO(QString("Start read settings in %1").arg(settingsPath->fileName()));

    Settings * settings = Settings::getInstance();
    settings->initSettings(*settingsPath);
    settings->readSettings();
    L_INFO("---------Settings info ------------");
    L_INFO(settings->paramListString());
    L_INFO("-----------------------------------");

    LoaderManager loaderManager;

    // close properly after treatment
    QObject::connect(&loaderManager, SIGNAL(closeAppSignal()), qApp, SLOT(quit()), Qt::QueuedConnection);

    loaderManager.launchUpdater();
    loaderManager.closeAppEvent();

    return app.exec();
}
