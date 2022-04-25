#ifndef LOGS_H
#define LOGS_H

#include <stdio.h>

#ifdef LOGGING
    #ifndef LOG_NAME
        #define LOG_NAME "GLOBAL"
    #endif

    static  const char *LogPath = "logs/Log" LOG_NAME ".log";
    static  FILE       *LogFile = fopen (LogPath, "at");
    #define LOG_MSG(msg, ...) LogMsg (LogFile, 0, msg, __FUNCTION__, __LINE__, __VA_ARGS__);
#else
    #define LogFile stderr
    #define LOG_MSG(msg, ...) ;
    #define OpenLogFile(name, mode) ;
    #define CloseLogFile(file) ;
#endif

#define LOG_ERR(msg, ...) LogMsg (LogFile, 1, msg, __FUNCTION__, __LINE__, __VA_ARGS__);

#define $                                                                       \
        {                                                                       \
            LOG_MSG ("<HR>\n"                                                   \
                    "%s at %s (%d)\n",                                          \
                  __FUNCTION__, __FILE__, __LINE__);                            \
        }

#define $$ LOG_MSG ("\t%s at %s (%d)\n", __FUNCTION__, __FILE__, __LINE__);

int LogMsg (FILE *logFile, char isErr, const char *format, const char *func, int line, ...);

#undef LOG_NAME
#endif
