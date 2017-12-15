#include <QApplication>
#include <QDebug>

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
    new Logger("downloader.log");


    L_INFO("Start Application");
    QApplication app(argc, argv);

    return app.exec();
} // main
