#include <QApplication>
#include <QDir>
#include <QtDebug>

#include "gui/mainwindow.h"
#include "gui/splashscreen.h"
#include "gui/style/stylemanager.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "settings/resourcessettings.h"
#include "settings/settings.h"
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
    qDebug() << "-- Installation root: " << appPath.getInstallationRootPath().absolutePath();

    new Logger(appPath.getLogsDirPath().absolutePath() + "/updater.log");

    L_INFO("Updater started.");

    QString settingsPath = appPath.getConfigurationFilePath();

    L_INFO("Start read install folder resources" + settingsPath);
    Settings * settings = Settings::getInstance();
    settings->initSettings(settingsPath);
    settings->readSettings();
    L_INFO("---------Settings info ------------");
    L_INFO(settings->paramListString());
    L_INFO("-----------------------------------");

    // init style
    StyleManager::setGeneralStyle();

    // init language with locale in settings
    LanguageManager::updateLanguage(LanguageManager::getStringLanguageFromEnum(settings->getLanguage()));

    QStringList args = qApp->arguments();
    args.removeFirst();
    if (args.contains("-debug")) {
        L_INFO("Updater Arguments = " + args.join(" "));
    }

//    Splashscreen splashscreen;
//    splashscreen.show();
//    splashscreen.center();

    MainWindow window;
    window.show();
    window.center();

    return app.exec();
} // main
