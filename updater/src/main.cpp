#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QtDebug>

#include "gui/mainwindow.h"
#include "gui/style/stylemanager.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "settings/commandlinesingleton.h"
#include "io/info.h"
#include "utils.h"

/*!
 *
 * \mainpage Stand-alone deployment
 *
 * \section intro_sec Introduction
 *
 * Stand-alone deployment lets you install applications and troubleshoot automatic updates
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Download sources
 *
 *
 * \version 2.0
 *
 * \author
 */

/*!
 * \file main.cpp
 *
 * \brief main
 * \param argc
 * \param argv
 * \return
 */
int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    // load settings resource static file. must be called keymanager_resources
    Q_INIT_RESOURCE(keymanager_resources);

    AppPath appPath = Utils::getAppPath();
    qDebug() << "-- Installation root: " << appPath.getInstallationDir().absolutePath();

    QLockFile lockFile(appPath.getInstallationDir().absoluteFilePath("lockfile"));
    if (!lockFile.tryLock(100)) {
        qDebug() << "-- Only one instance permitted, exiting.";
        return 0;
    }

    new Logger(appPath.getLogsDir().absoluteFilePath("updater.log"));

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

    L_INFO("Updater started.");

    QSharedPointer<QFile> settingsPath = appPath.getConfigurationFile();

    L_INFO(QString("Start read install folder resources: %1").arg(settingsPath->fileName()));
    Settings * settings = Settings::getInstance();
    settings->initSettings(*settingsPath);
    settings->readSettings();
    L_INFO("---------Settings info ------------");
    L_INFO(settings->paramListString());
    L_INFO("-----------------------------------");

    // init style
    StyleManager::setGeneralStyle();

    // init language with locale in settings
    LanguageManager::updateLocale(settings->getLocale());
    L_INFO(QString("Updated locale with: %1").arg(settings->getLocale()));

    arguments.removeFirst();
    CommandLineSingleton::getInstance()->setArguments(arguments);

    if (CommandLineSingleton::getInstance()->isDebugMode()) {
        L_INFO(QString("Updater Arguments: %1").arg(CommandLineSingleton::getInstance()->getAllArguments().join(" ")));
        L_INFO(QString("Query Arguments: %1").arg(CommandLineSingleton::getInstance()->getApplicationHttpArguments()));
    }

    MainWindow window;
    window.show();
    window.center();

    return app.exec();
}
