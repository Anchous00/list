#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

#ifdef LOG_ON
#define ON_LOG(...) __VA_ARGS__
#else
#define ON_LOG(...)
#endif

enum log_status
{
    LOG_INFO    = 1,
    LOG_ERROR   = 2,
    LOG_ACTION  = 3,
    LOG_DEFAULT = 4
};

const int MAX_LOG_MESSAGE_LEN = 100;

#define LOG(LOG_STATUS, format_string, ...) do                                                  \
        {                                                                           \
        char log_message[MAX_LOG_MESSAGE_LEN] = {};                                 \
        snprintf(log_message, MAX_LOG_MESSAGE_LEN, format_string, ##__VA_ARGS__);     \
        log(LOG_STATUS, log_message);                                               \
        } while (0);

extern FILE *LOG_FILE;

void log( log_status status, const char* log_message );
void log_file_open( const char* log_file_name );
void log_file_close( void );

#endif
