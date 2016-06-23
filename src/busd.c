/*
 * busd.c
 *
 *  Created on: Jun 20, 2016
 *      Author: jnevens
 */
#include <stdio.h>
#include <argp.h>
#include <stdlib.h>
#include <unistd.h>

#include <bus/event.h>
#include <bus/daemon.h>
#include <bus/log.h>
#include <bus/socket.h>

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

void busd_connection_callback(int fd, void *arg)
{
	uint8_t buf[256];
	socket_t *client = arg;
	int rv = socket_read(client, buf, sizeof(buf));

	if(rv == 0) {
		log_info("Connection closed!");
		event_t *event = socket_get_userdata(client);
		event_destroy(event);
	} else {
		log_info("[%d] %s", rv, (const char *)buf);
	}
}

void busd_server_callback(int fd, void *arg)
{
	socket_t *server = arg;
	log_info("server callback!");
	socket_t *new = socket_accept(server);
	event_t *event = event_add(socket_get_fd(new), busd_connection_callback, NULL, new);
	socket_set_userdata(new, event);
}

int main(int argc, char *argv[])
{
	log_init("busd");
	event_loop_init();

	log_info("hello!");

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if (arguments.verbose) {
		log_set_print_level(BLOG_DEBUG);
	}

	if (arguments.daemonize) {
		daemonize("/var/run/busd.pid");
	}

	socket_t *server = socket_create_unix();
	unlink("/var/run/busd.sock");
	socket_bind_unix(server, "/var/run/busd.sock");
	socket_listen(server, 10);
	log_debug("fd = %d", socket_get_fd(server));
	event_add(socket_get_fd(server), busd_server_callback, NULL, server);

	event_loop();
	event_loop_cleanup();
	return 0;
}

