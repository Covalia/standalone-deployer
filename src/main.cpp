#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QString>

#include "../ui/windows.h"
#include "../ui/welcome.h"
#include "../ui/personnalize.h"
#include "mainwindow.h"
#include "shortcut/shortcut.h"
#include "log/simpleQtLogger.h"
#include "log/logger.h"
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
int main(int argc, char * argv[]){
    // logger initialization
    new Logger(QString("C:\\Users\\Alexis\\Desktop\\test logs\\logDeploiment.log"));

    L_INFO("Start Application");

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

    QApplication app(argc, argv);

    LanguageManager::initLanguage();
//    LanguageManager::updateLanguage("en_US");
//    LanguageManager::updateLanguage("fr_FR");

    Windows window;
    window.show();
    window.center();

//    Welcome welcom;
//    window.changeContentWidget(&welcom);

    Personnalize perso;
    window.changeContentWidget(&perso);

   QObject::connect(&window, &Windows::changeLanguageSignal, &window, &Windows::changeLanguage);
   QObject::connect(&window, &Windows::changeLanguageSignal, &perso, &Personnalize::changeLanguage);

    return app.exec();
} // main
