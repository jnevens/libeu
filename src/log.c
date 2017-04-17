#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>

#include "eu/log.h"

static int log_print_lvl = BLOG_DEBUG;
static int log_syslog_lvl = BLOG_ERR;

/*
 * Initialize logging
 */
int log_init(const char *name){
	setlogmask(LOG_UPTO (LOG_DEBUG));
	openlog(name, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
	return 0;
}

/*
 * exit logging
 */
int log_exit(void){
	closelog();
	return 0;
}

int logs(int lvl, const char *func, int line, const char *fmt, ...){
	va_list ap;
	char *str,*strlog,lvlstr[12];
	int rv = 0;
	va_start(ap, fmt);
	rv = vasprintf(&str,fmt,ap);
	if(rv < 0){
		Log(BLOG_ERR,"Could not allocate memory!");
	}
	va_end(ap);

	switch(lvl){
		case 0 : strcpy(lvlstr,"FATAL"); break;
		case 1 : strcpy(lvlstr,"ALERT"); break;
		case 2 : strcpy(lvlstr,"CRITIC"); break;
		case 3 : strcpy(lvlstr,"ERROR"); break;
		case 4 : strcpy(lvlstr,"WARNING"); break;
		case 5 : strcpy(lvlstr,"NOTICE"); break;
		case 6 : strcpy(lvlstr,"INFO"); break;
		case 7 : strcpy(lvlstr,"DEBUG"); break;
		default : return -1;
	}

	rv = asprintf(&strlog,"[%s] %s(%d): %s",lvlstr,func,line,str);
	if(rv < 0){
		Log(BLOG_ERR,"Could not allocate memory!");
	}
	if(lvl <= log_print_lvl)
		printf("%s\n",strlog);
	if(lvl <= log_syslog_lvl)
		syslog(lvl,"%s",strlog);
	fflush(NULL);
	free(strlog);
	free(str);
	if(lvl == 0)
		exit(-1);
	return 0;
}

void log_set_print_level(int level){
	log_print_lvl = level;
}

void log_set_syslog_level(int level){
	log_syslog_lvl = level;
}
