#ifndef LOGGER_LOGGER_H
#define LOGGER_LOGGER_H

#include "log/simpleqtlogger.h"

const unsigned int LOG_FILES_MAX = 20; // max rolling files
const unsigned int LOG_FILE_SIZE = 10000000; // size log file in byte

class Logger
{
    public:
        Logger(QString pathLog);
};

#endif
