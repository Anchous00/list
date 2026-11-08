#include "logger.h"

FILE *LOG_FILE = NULL;

// LOG ( log_status status, const char* format_str, __VA_ARGS__)

void log( log_status status, const char* log_message )
{
    time_t t = time(NULL);
    struct tm *time_info = localtime(&t);

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);  // или timespec_get(&ts, TIME_UTC)


    char time_str[64] = {};
    strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M:%S", time_info);
    char time_s[256] = {};
    snprintf(time_s, sizeof(time_s), "%s.%ld", time_str, ts.tv_nsec);
    if (LOG_FILE == NULL)
    {
        LOG_FILE = stderr;
    }

    if (status != myLOG_DEFAULT) fprintf(LOG_FILE, "[%s]\t", time_s);

    switch (status)
    {
        case myLOG_INFO:
            fprintf(LOG_FILE, "INFO");
            break;
        case myLOG_ERROR:
            fprintf(LOG_FILE, "ERROR");
            break;
        case myLOG_ACTION:
            fprintf(LOG_FILE, "ACTION");
            break;
        case myLOG_DEFAULT:
            break;
        default:
            break;
    }
    fprintf(LOG_FILE, "\t%s\n", log_message);

    return;
}

void log_file_open( const char* log_file_name )
{
    LOG_FILE = fopen(log_file_name, "w");
    if (LOG_FILE == NULL)
        fprintf(stderr, "ERROR: can't open %s log file\n", log_file_name);
    log(myLOG_DEFAULT, "<!DOCTYPE html>\n<link rel=\"stylesheet\" href=\"styles.css\">");
    log(myLOG_ACTION, "Log start\n\n");
    return;
}

void log_file_close( void )
{
    log(myLOG_ACTION, "Log end");

    if (fclose(LOG_FILE) != 0)
        fprintf(LOG_FILE, "ERROR: can't close log file stream\n");

    return;
}

