#ifndef INCLUDE_BUS_LOG_H_
#define INCLUDE_BUS_LOG_H_

/*! \file logger.h
 * Logger is used to log all kind of messages, the most important messages are stored 
 * in the local syslog of the target systems!
 * @author Jasper Nevens <jasper.nevens@emco-services.be>
 */

#define Log(lvl, fmt, ...) logs(lvl, __func__ , __LINE__ , fmt, ##__VA_ARGS__)

/* Macro's for easy use of log */
#define log_fatal(fmt, ...) Log(BLOG_FATAL, fmt, ##__VA_ARGS__)
#define log_alert(fmt, ...) Log(BLOG_ALERT, fmt, ##__VA_ARGS__)
#define log_crit(fmt, ...) Log(BLOG_CRIT, fmt, ##__VA_ARGS__)
#define log_err(fmt, ...) Log(BLOG_ERR, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) Log(BLOG_WARNING, fmt, ##__VA_ARGS__)
#define log_notice(fmt, ...) Log(BLOG_NOTICE, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) Log(BLOG_INFO, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) Log(BLOG_DEBUG, fmt, ##__VA_ARGS__)

/* 
 * Enumerate description of log level
 */
enum log_levels
{
	BLOG_NOLOG = -1,
	BLOG_FATAL = 0,
	BLOG_ALERT,
	BLOG_CRIT,
	BLOG_ERR,
	BLOG_WARNING,
	BLOG_NOTICE,
	BLOG_INFO,
	BLOG_DEBUG,
};

/*! init logging, open syslog connection etc...
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param name syslog name
 * @return 0 on succes, 1 on failure
 */
int log_init(const char *name);

/*! exit logging, close syslog connection etc...
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @return 0 on succes, 1 on failure
 */
int log_exit(void);

/*! make log entry
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param lvl Importance of log message, from BLOG_FATAL downto BLOG_DEBUG
 * @param func name of the function to display in syslog
 * @param line number of the line in the src file
 * @param fmt format of the log message
 * @param ... all values from the log message
 * @return returns 0 on succes, -1 on failure!
 */
int logs(int lvl, const char *func, int line, const char *fmt, ...);

/*! set syslog log level
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param level minimum syslog level (from logger_levels)
 */
void log_set_syslog_level(int level);

/*! set print log level
 * @author Jasper Nevens <jasper.nevens@tass.be>
 * @param level minimum syslog level (from logger_levels)
 */
void log_set_print_level(int level);


#endif
