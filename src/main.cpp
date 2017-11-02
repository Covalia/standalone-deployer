#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include <QString>

#include "mainwindow.h"
#include "log/simpleQtLogger.h"
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
int main(int argc, char * argv[]){
    // logger initialization
    Logger * log = new Logger(QString("C:\\Users\\Alexis\\Desktop\\test logs\\logDeploiment.log"));

    L_INFO("Start Application");

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
}
