#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QString>
#include <QIcon>

#include "uimanager/uimanager.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "language/languagemanager.h"

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
    // TODO set path installation? Must be init before installation to log
    // logger initialization
    new Logger(QString("C:\\Users\\Alexis\\Desktop\\test logs\\logDeploiment.log"));

    L_INFO("Start Application");
    QApplication app(argc, argv);

    // TODO set path of installation, and remove in installation manager
    Settings * settings = Settings::getInstance();
    QString installPath("P:/DEV/Workspace_downloader/downloader/test.ini");
    settings->initSettings(installPath);
    settings->setProxyPassword("hello@password!étoile?*");
    settings->writeSettings();
    settings->readSettings();
    L_INFO("Password = " + settings->getProxyPassword());

    app.setWindowIcon(QIcon(":/resources/icon.png"));
    app.setApplicationName(QString(QObject::tr("Stand-alone deployment")));

    LanguageManager::initLanguage();

    UIManager * uiManager = new UIManager();
    uiManager->init();

    return app.exec();
} // main
