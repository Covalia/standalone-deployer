#include <QApplication>
#include <QTranslator>
#include <QDebug>

#include "gui/mainwindow.h"
#include "log/logger.h"

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
    new Logger("updater.log");


    L_INFO("Downlaoder started.");
    QApplication app(argc, argv);

    QTranslator translator;
    // TODO gérer langue par défaut et demander quelle langue utiliser.
    QString locale = QLocale::system().name();
    QString location = ":/translations";
    qDebug() << "locale:" << locale;

    qDebug() << "loading translator";
    translator.load(locale, location);
    app.installTranslator(&translator);

    MainWindow window;
    window.show();
    window.center();

    return app.exec();
} // main
