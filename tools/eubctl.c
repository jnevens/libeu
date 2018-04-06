/*
 * client.c
 *
 *  Created on: Jun 23, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <string.h>

#include <eu/event.h>
#include <eu/client.h>
#include <eu/object.h>
#include <eu/log.h>

static char *cwd = NULL;

/* Used by main to communicate with parse_opt. */
struct arguments
{
	bool verbose;
};

/* Default arguments */
struct arguments arguments = {
	.verbose = false,
};

/* Program documentation. */
static char doc[] = "busd dispatch daemon";

/* A description of the arguments we accept. */
static char args_doc[] = "";

/* The options we understand. */
static struct argp_option options[] = {
	{ "verbose", 'v', 0, 0, "Verbose output" },
	{ 0 }
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	 know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key)
	{
		case 'v':
			arguments->verbose = true;
			break;
		case ARGP_KEY_ARG:
			break;
		case ARGP_KEY_END:
			//argp_usage(state);
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

static void eubctl_help_print()
{
	printf("Available commands:\n");
	printf("\thelp         print help\n");
	printf("\tls [path]    list objects\n");
	printf("\tcd [path]    enter location\n");
	printf("\tpwd          current location\n");
}

static void eubctl_list_object()
{

}

static void eubctl_change_directory(const char *buf)
{
	if (!strcmp(buf,"cd")) {
		free(cwd);
		cwd = strdup("");
	} else if (strlen(buf) > 3) {
		if (strlen(cwd) > 0) {
			char *tmp = strdup(cwd);
			size_t newlen = strlen(cwd) + strlen(&buf[3]) + 2;
			cwd = realloc(cwd, newlen);
			snprintf(cwd, newlen, "%s.%s", tmp, &buf[3]);
			free(tmp);
		} else {
			free(cwd);
			cwd = strdup(&buf[3]);
		}
	}
}

void eubctl_list(eu_bus_conn_t *conn, const char *cwd)
{
	eu_bus_status_t status;
	eu_object_t *obj = eu_bus_list_object(conn, cwd, &status);

	if (status == EU_BUS_SUCCESS) {
		eu_object_print(obj, eu_object_print_attrs_none);
	} else if (status == EU_BUS_NO_SUCH_OBJECT) {
		printf("Error: No such object found!\n");
	}
}

static void eubctl_read_cmd(int fd, short int revents, void *arg)
{
	eu_bus_conn_t *conn = arg;
	char buf[256];
	fgets(buf, sizeof(buf), stdin);

	size_t len = strlen(buf);
	buf[len -1] = '\0';

	if (!strcmp(buf, "help")) {
		eubctl_help_print();
	} else if (!strcmp(buf, "ls")) {	/* list children and parameters in this directory */
		eubctl_list(conn, cwd);
	} else if (!strncmp(buf, "cd", 2)) {	/* change directory */
		eubctl_change_directory(buf);
	} else if (!strcmp(buf, "pwd")) {	/* print working directory */
		printf("%s\n",cwd);
	} else {
		printf("Unknown command!\n");
	}
}

int main(int argc, char *argv[])
{
	eu_log_init("busd");
	eu_event_loop_init();

	cwd = strdup("");

	eu_log_info("euctl start!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		eu_log_set_print_level(EU_LOG_DEBUG);
	}

	eu_bus_conn_t *conn = eu_bus_connect("eubctl");
	if(!conn) {
		eu_log_err("Failed to connect to system bus");
	}

	eu_event_add(2, POLLIN, eubctl_read_cmd, NULL, conn);

	eu_event_loop();
	eu_event_loop_cleanup();
	return 0;
}
