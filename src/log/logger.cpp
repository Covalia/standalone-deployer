#include "logger.h"

#include "simpleQtLogger.h"
#include <QApplication>

/*!
 * \brief Initializes application logger
 * \param pathLog
 */
Logger::Logger(QString pathLog){
    // enable sinks
    simpleqtlogger::ENABLE_LOG_SINK_FILE = true;
    simpleqtlogger::ENABLE_LOG_SINK_CONSOLE = true;
    simpleqtlogger::ENABLE_LOG_SINK_QDEBUG = false;
    simpleqtlogger::ENABLE_LOG_SINK_SIGNAL = true;

    // set log-features
    simpleqtlogger::ENABLE_FUNCTION_STACK_TRACE = true;
    simpleqtlogger::ENABLE_CONSOLE_COLOR = true;

    // set log-levels (global; all enabled)
    simpleqtlogger::ENABLE_LOG_LEVELS.logLevel_DEBUG = true;
    simpleqtlogger::ENABLE_LOG_LEVELS.logLevel_FUNCTION = true;

    // set log-levels (specific)
    simpleqtlogger::EnableLogLevels enableLogLevels_file = simpleqtlogger::ENABLE_LOG_LEVELS;
    simpleqtlogger::EnableLogLevels enableLogLevels_console = simpleqtlogger::ENABLE_LOG_LEVELS;
    simpleqtlogger::EnableLogLevels enableLogLevels_qDebug = simpleqtlogger::ENABLE_LOG_LEVELS;
    simpleqtlogger::EnableLogLevels enableLogLevels_signal = simpleqtlogger::ENABLE_LOG_LEVELS;
    enableLogLevels_console.logLevel_FUNCTION = false;

    simpleqtlogger::SimpleQtLogger::createInstance(qApp)->setLogFileName(pathLog, LOG_FILE_SIZE, LOG_FILES_MAX);
    simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_file(enableLogLevels_file);
    simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_console(enableLogLevels_console);
    simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_qDebug(enableLogLevels_qDebug);
    simpleqtlogger::SimpleQtLogger::getInstance()->setLogLevels_signal(enableLogLevels_signal);

    L_INFO("Log initialization completed");
}
