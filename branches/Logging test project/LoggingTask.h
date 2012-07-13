/* Gerards simple logging system, inspired by syslog */
#ifndef LOGGING_TASK_H
#define LOGGING_TASK_H

/* Maximum amount of apps to register to logging */
#define LOG_APP_AMOUNT 10
#define LOG_MAX_APP_NAME 15

/* 
===Log levels===
This determines the importance of the message. The levels are, in order of decreasing importance:
LOG_EMERG - system is unusable
LOG_ALERT - action must be taken immediately
LOG_CRIT - critical conditions
LOG_ERR - error conditions
LOG_WARNING - warning conditions
LOG_NOTICE - normal, but significant, condition
LOG_INFO - informational message
LOG_DEBUG - debug-level message 
(Same loglevels used as defined in syslog manpage)
*/
typedef enum {LOG_DEBUG,LOG_INFO,LOG_NOTICE,LOG_WARNING,LOG_ERR,LOG_CRIT,LOG_ALERT,LOG_EMERG} loglevels;

/* Logging app struct */
typedef struct
{
    unsigned char ucId;
    unsigned char uacApp[LOG_MAX_APP_NAME];
    loglevels logLevel;       
} logtable;

void logInit();
unsigned char registerApp(unsigned char *uacApp, loglevels logLevel);
unsigned char deregisterApp(unsigned char ucAppId);
void writeLog(unsigned char ucAppId, unsigned char *uacLogMessage, loglevels logLevel);
void cmd_printLogTable();

#endif
