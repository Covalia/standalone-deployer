#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QString>

#include "mainwindow.h"
#include "uiManager/uimanager.h"
#include "shortcut/shortcut.h"
#include "log/simpleqtlogger.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "commandLineParser/commandlineparser.h"
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
    // logger initialization
    new Logger(QString("C:\\Users\\Alexis\\Desktop\\test logs\\logDeploiment.log"));

    L_INFO("Start Application");
    QApplication app(argc, argv);

    L_INFO("Start Commande Lien Parser");
    new CommandLineParser();

    Settings& settings=Settings::Instance();
    QString installPath("P:/DEV/Workspace_downloader/downloader/test.ini");
    settings.initSettings(installPath);
    settings.writeSettings();

#ifdef Q_OS_WIN
        Shortcut * shorcutCreator = new Shortcut();
        LPCWSTR target_file = L"C:/Program Files (x86)/Opera/launcher.exe";
        LPCWSTR target_args = L"";
        LPSTR link_file = "C:/Users/Alexis/Desktop/covotem.lnk";
        LPCWSTR description = L"This is description";
        LPCWSTR cur_dir = L"C:/Users/Alexis/Desktop/";
        LPCWSTR icon_file = L"C:/Users/Alexis/Desktop/ido.ico";

        shorcutCreator->createWindowsShortcut(target_file, target_args,
                                              link_file, description,
                                              1, cur_dir,
                                              icon_file, 0);
        QString allUserStartMenu = shorcutCreator->findAllUserStartMenuFolder();
        QString userStartMenu = shorcutCreator->findUserStartMenuFolder();
		
        L_INFO(userStartMenu);
#endif

    app.setWindowIcon(QIcon(":/resources/icon.png"));
    app.setApplicationName(QString(QObject::tr("Stand-alone deployment")));
	
    LanguageManager::initLanguage();

    UIManager * uiManager = new UIManager();
    uiManager->changeWelcome();

    return app.exec();
} // main
