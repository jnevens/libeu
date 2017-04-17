#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <eu/log.h>
#include <eu/daemon.h>

static int daemon_write_pidfile(const char *pidfile, pid_t pid)
{
	FILE * fp;
	fp = fopen(pidfile, "w+");
	if(!fp){
		log_err("Failed opening PID file!");
		return -1;
	}
	if(fprintf(fp,"%d\n",(int)pid) <= 0){
		log_err("Failed writing pid to file!");
	}
	fclose(fp);
	return 0;
}

static int daemon_remove_pidfile(const char *pidfile)
{
	return (pidfile) ? unlink(pidfile) : -1;
}

bool daemonize(const char *pf)
{
	/* Our process ID and Session ID */
	pid_t pid, sid;

	/* Remove pidfile if exists */
	daemon_remove_pidfile(pf);

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}
	/* If we got a good PID, then we can exit the parent process. */
	if (pid > 0) { // Child can continue to run even after the parent has finished executing
		log_debug("Child ceated: pid: %d", pid);
		daemon_write_pidfile(pf, pid);
		exit(EXIT_SUCCESS);
	}

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory */
	if ((chdir("/")) < 0) {
		/* Log the failure */
		exit(EXIT_FAILURE);
	}

	/* Close out the standard file descriptors */
	//Because daemons generally dont interact directly with user so there is no need of keeping these open
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return true;
}
