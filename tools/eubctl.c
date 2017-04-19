/*
 * client.c
 *
 *  Created on: Jun 23, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>

#include <eu/event.h>
#include <eu/client.h>
#include <eu/log.h>

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

int main(int argc, char *argv[])
{
	eu_log_init("busd");
	eu_event_loop_init();

	log_info("euctl start!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		eu_log_set_print_level(EU_LOG_DEBUG);
	}

	eu_bus_conn_t *conn = eu_bus_connect("eubctl");
	if(!conn) {
		log_err("Failed to connect to system bus");
	}
	eu_object_t *root = eu_bus_register_path(conn, "system.deamon.test");
	if(!root) {
		log_err("Failed to register object Devices");
	} else {
		eu_object_print(root);
	}
	eu_event_loop();
	eu_event_loop_cleanup();
	return 0;
}





