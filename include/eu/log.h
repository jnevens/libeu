#ifndef INCLUDE_BUS_LOG_H_
#define INCLUDE_BUS_LOG_H_

/*! \file logger.h
 * Logger is used to log all kind of messages, the most important messages are stored 
 * in the local syslog of the target systems!
 * @author Jasper Nevens <jasper.nevens@emco-services.be>
 */

#define EU_Log(lvl, fmt, ...) eu_logs(lvl, __func__ , __LINE__ , fmt, ##__VA_ARGS__)

/* Macro's for easy use of log */
#define eu_log_fatal(fmt, ...) EU_Log(EU_LOG_FATAL, fmt, ##__VA_ARGS__)
#define eu_log_alert(fmt, ...) EU_Log(EU_LOG_ALERT, fmt, ##__VA_ARGS__)
#define eu_log_crit(fmt, ...) EU_Log(EU_LOG_CRIT, fmt, ##__VA_ARGS__)
#define eu_log_err(fmt, ...) EU_Log(EU_LOG_ERR, fmt, ##__VA_ARGS__)
#define eu_log_warn(fmt, ...) EU_Log(EU_LOG_WARNING, fmt, ##__VA_ARGS__)
#define eu_log_notice(fmt, ...) EU_Log(EU_LOG_NOTICE, fmt, ##__VA_ARGS__)
#define eu_log_info(fmt, ...) EU_Log(EU_LOG_INFO, fmt, ##__VA_ARGS__)
#define eu_log_debug(fmt, ...) EU_Log(EU_LOG_DEBUG, fmt, ##__VA_ARGS__)

/* 
 * Enumerate description of log level
 */
enum eu_log_levels
{
	EU_LOG_NOLOG = -1,
	EU_LOG_FATAL = 0,
	EU_LOG_ALERT,
	EU_LOG_CRIT,
	EU_LOG_ERR,
	EU_LOG_WARNING,
	EU_LOG_NOTICE,
	EU_LOG_INFO,
	EU_LOG_DEBUG,
};

/*! init logging, open syslog connection etc...
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param name syslog name
 * @return 0 on succes, 1 on failure
 */
int eu_log_init(const char *name);

/*! exit logging, close syslog connection etc...
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @return 0 on succes, 1 on failure
 */
int log_exit(void);

/*! make log entry
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param lvl Importance of log message, from EU_LOG_FATAL downto EU_LOG_DEBUG
 * @param func name of the function to display in syslog
 * @param line number of the line in the src file
 * @param fmt format of the log message
 * @param ... all values from the log message
 * @return returns 0 on succes, -1 on failure!
 */
int eu_logs(int lvl, const char *func, int line, const char *fmt, ...);

/*! set syslog log level
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param level minimum syslog level (from logger_levels)
 */
void eu_log_set_syslog_level(int level);

/*! set print log level
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param level minimum syslog level (from logger_levels)
 */
void eu_log_set_print_level(int level);


#endif
