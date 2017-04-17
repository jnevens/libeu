/*
 * eud.c
 *
 *  Created on: Jun 20, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <unistd.h>

#include <eu/event.h>
#include <eu/daemon.h>
#include <eu/log.h>
#include <eu/socket.h>
#include <eu/bus_message.h>

/* Used by main to communicate with parse_opt. */
struct arguments
{
	bool daemonize;
	bool verbose;
};

/* Default arguments */
struct arguments arguments = {
	.daemonize = false,
	.verbose = false,
};

/* Program documentation. */
static char doc[] = "busd dispatch daemon";

/* A description of the arguments we accept. */
static char args_doc[] = "";

/* The options we understand. */
static struct argp_option options[] = {
	{ "deamonize", 'd', 0, 0, "Daemonize" },
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
		case 'd':
			arguments->daemonize = true;
			break;
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

void busd_connection_handler(eu_event_t *event, eu_bus_message_t *msg)
{
	switch(msg->type) {
		case REGISTER_PATH :
			log_info("Register path: %s", (char *)msg->data);
			break;
		default:
			log_err("Unsupported action: %d", msg->type);
			break;
	}
}

void busd_connection_callback(int fd, short int revents, void *arg)
{
	size_t msg_header_size = sizeof(eu_bus_message_t);
	eu_bus_message_t *msg = calloc(1, msg_header_size);
	eu_socket_t *client = arg;
	eu_event_t *event = eu_socket_get_userdata(client);

	// read header
	int rv = eu_socket_read(client, (uint8_t *)msg, msg_header_size);
	if(rv != msg_header_size) {
		log_err("Failed reading header!");
	}
	// read data
	msg = realloc(msg, msg_header_size + msg->len);
	rv = eu_socket_read(client, &((uint8_t *)msg)[msg_header_size], msg->len);
	if(rv != msg->len) {
		log_err("Failed reading message data");
	}

	if(rv == 0) {
		log_info("Connection closed!");
		eu_event_destroy(event);
	} else {
		busd_connection_handler(event, msg);
	}
}

void busd_server_callback(int fd, short int revents, void *arg)
{
	eu_socket_t *server = arg;
	log_info("server callback!");
	eu_socket_t *new = eu_socket_accept(server);
	eu_event_t *event = eu_event_add(eu_socket_get_fd(new), POLLIN, busd_connection_callback, NULL, new);
	eu_socket_set_userdata(new, event);
}

int main(int argc, char *argv[])
{
	eu_log_init("busd");
	eu_event_loop_init();

	log_info("hello!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		eu_log_set_print_level(EU_LOG_DEBUG);
	}

	if (arguments.daemonize) {
		eu_daemonize("/var/run/busd.pid");
	}

	eu_socket_t *server = eu_socket_create_unix();
	if(!server) {
		log_err("Failed creating server socket!");
		exit(-1);
	}
	unlink("/var/run/busd.sock");
	if(!eu_socket_bind_unix(server, "/var/run/busd.sock")) {
		exit(-1);
	}
	eu_socket_listen(server, 10);
	log_debug("fd = %d", eu_socket_get_fd(server));
	eu_event_add(eu_socket_get_fd(server), POLLIN, busd_server_callback, NULL, server);

	eu_event_loop();
	eu_event_loop_cleanup();
	return 0;
}

