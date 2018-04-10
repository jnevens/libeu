/*
 * eubdummy.c
 *
 *  Created on: Feb 3, 2018
 *      Author: jnevens
 */
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>

#include <eu/event.h>
#include <eu/client.h>
#include <eu/object.h>
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

static void eu_bus_disconnect_cb(eu_bus_conn_t *conn, void *arg)
{
	eu_log_err("eubus disconnected, closing application!");
	eu_event_loop_stop();
}

int main(int argc, char *argv[])
{
	eu_log_init("busd");
	eu_event_loop_init();

	eu_log_info("euctl start!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		eu_log_set_print_level(EU_LOG_DEBUG);
	}

	eu_bus_conn_t *conn = eu_bus_connect("eubctl");
	if(!conn) {
		eu_log_err("Failed to connect to system bus");
	}
	eu_bus_set_disconnect_handler(conn, eu_bus_disconnect_cb, NULL);
	eu_object_t *root = eu_bus_register_client(conn, "Dummy");
	if(!root) {
		eu_log_err("Failed to register object Devices");
	} else {
		eu_object_t *parent = eu_object_create(root, "com", eu_object_attr_none);
		eu_object_t *child1 = eu_object_create(parent, "dell", eu_object_attr_none);
		eu_object_t *child1_1 = eu_object_create(child1, "www", eu_object_attr_none);
		eu_object_t *child2 = eu_object_create(parent, "hp", eu_object_attr_none);
		eu_object_t *child2_1 = eu_object_create(child2, "www", eu_object_attr_none);
		eu_object_t *child3 = eu_object_create(parent, "msi", eu_object_attr_none);
		eu_object_t *child3_1 = eu_object_create(child3, "www", eu_object_attr_none);
		eu_object_t *child4 = eu_object_create(parent, "lenovo", eu_object_attr_none);
		eu_object_t *child4_1 = eu_object_create(child4, "www", eu_object_attr_none);
		eu_object_parameter_set_bool(child1_1, "https", true);
		eu_object_parameter_set_bool(child1_1, "tls", true);
		eu_object_parameter_set_int32(child1_1, "visitors", 8098099);
		eu_object_parameter_set_bool(child2_1, "https", true);
		eu_object_parameter_set_bool(child2_1, "tls", false);
		eu_object_parameter_set_int32(child2_1, "visitors", 1869);
		eu_object_parameter_set_bool(child3_1, "https", true);
		eu_object_parameter_set_bool(child3_1, "tls", false);
		eu_object_parameter_set_int32(child3_1, "visitors", 8984);
		eu_object_parameter_set_bool(child4_1, "https", true);
		eu_object_parameter_set_bool(child4_1, "tls", false);
		eu_object_parameter_set_int32(child4_1, "visitors", 28983);
	}
	eu_event_loop();
	eu_event_loop_cleanup();
	return 0;
}





