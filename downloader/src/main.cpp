#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QString>
#include <QIcon>

#include "uimanager/uimanager.h"
#include "shortcut/shortcut.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "settings/resourcessettings.h"
#include "commandline/commandlineparser.h"
#include "language/languagemanager.h"
#include "style/stylemanager.h"

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

    //init project resources
    ResourcesSettings * projectSetting = ResourcesSettings::getInstance();
    projectSetting->initSettings(":/resources/project.ini");
    projectSetting->readSettings();
    projectSetting->sendToSettings();
    L_INFO("Ip=" + projectSetting->getIp_server());

    L_INFO("Start Application");
    QApplication app(argc, argv);

    // init style
    StyleManager::setGeneralStyle();

    L_INFO("Start Commande Lien Parser");
    CommandLineParser * lineParser = new CommandLineParser();
    lineParser->sendToSettings();

    // TODO set path of installation, and remove in installation manager
    Settings * settings = Settings::getInstance();
    QString installPath("P:/DEV/Workspace_downloader/downloader/test.ini");
    settings->initSettings(installPath);
    settings->setProxyPassword("hello@password!étoile?*");
    settings->writeSettings();
    settings->readSettings();
    L_INFO("Password = " + settings->getProxyPassword());

    // TODO remove this in end of installation manager
#ifdef _WIN32
        Shortcut * shorcutCreator = new Shortcut();
        LPCWSTR target_file = L"C:/Program Files (x86)/Opera/launcher.exe";
        LPCWSTR target_args = L"";
        LPSTR link_file = "C:/Users/Alexis/Desktop/covotem.lnk";
        LPCWSTR description = L"This is description";
        LPCWSTR cur_dir = L"C:/Users/Alexis/Desktop/";
        LPCWSTR icon_file = L":/resources/images/icon.png";
        shorcutCreator->createWindowsShortcut(target_file, target_args,
                                              link_file, description,
                                              1, cur_dir,
                                              icon_file, 0);
        QString allUserStartMenu = shorcutCreator->findAllUserStartMenuFolder();
        QString userStartMenu = shorcutCreator->findUserStartMenuFolder();
        L_INFO(userStartMenu);
#endif

    app.setWindowIcon(QIcon(":/resources/images/icon.png"));
    app.setApplicationName(QString(QObject::tr("Stand-alone deployment")));

    LanguageManager::initLanguage();

    UIManager * uiManager = new UIManager();
    uiManager->init();

    return app.exec();
} // main
