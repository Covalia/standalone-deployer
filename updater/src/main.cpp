#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QtDebug>

#include "gui/mainwindow.h"
#include "gui/splashscreen.h"
#include "gui/style/stylemanager.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "settings/resourcessettings.h"
#include "settings/settings.h"
#include "settings/commandlinesingleton.h"
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

    AppPath appPath = Utils::getAppPath();
    qDebug() << "-- Installation root: " << appPath.getInstallationDir().absolutePath();

    QLockFile lockFile(appPath.getInstallationDir().absoluteFilePath("lockfile"));
    if (!lockFile.tryLock(100)) {
        qDebug() << "-- Only one instance permitted, exiting.";
        return 0;
    }

    new Logger(appPath.getLogsDir().absoluteFilePath("updater.log"));

    L_INFO("Updater started.");

    QSharedPointer<QFile> settingsPath = appPath.getConfigurationFile();

    L_INFO("Start read install folder resources" + settingsPath->fileName());
    Settings * settings = Settings::getInstance();
    settings->initSettings(*settingsPath);
    settings->readSettings();
    L_INFO("---------Settings info ------------");
    L_INFO(settings->paramListString());
    L_INFO("-----------------------------------");

    // init style
    StyleManager::setGeneralStyle();

    // init language with locale in settings
    LanguageManager::updateLanguage(LanguageManager::getStringLanguageFromEnum(settings->getLang()));

    QStringList arguments = qApp->arguments();
    arguments.removeFirst();
    CommandLineSingleton::getInstance()->setArguments(arguments);

    if (CommandLineSingleton::getInstance()->isDebugMode()) {
        L_INFO("Updater Arguments: " + CommandLineSingleton::getInstance()->getAllArguments().join(" "));
        L_INFO("Query Arguments: " + CommandLineSingleton::getInstance()->getApplicationHttpArguments());
    }

    MainWindow window;
    window.show();
    window.center();

    return app.exec();
} // main
