#ifndef LOGGER__LOGGER_H
#define LOGGER__LOGGER_H

#include "log/simpleqtlogger.h"

const unsigned int LOG_FILES_MAX = 20; // max rolling files
const unsigned int LOG_FILE_SIZE = 200000; // size log file in byte

class Logger
{
    public:
        Logger(QString pathLog);
};

#endif // LOGGER__LOGGER_H
