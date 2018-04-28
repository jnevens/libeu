AM_CPPFLAGS = -I$(top_buildir)/include -I$(top_srcdir)/include
AM_CFLAGS = -D_GNU_SOURCE

lib_LTLIBRARIES = libeu.la

libeu_la_LIBADD =
libeu_la_LDFLAGS = -ljson-c

libeu_la_SOURCES = \
	bus_client.c \
	bus_message.c \
	conffile.c \
	daemon.c \
	event.c \
	list.c \
	log.c \
	object.c \
	parameter.c \
	socket.c \
	string.c \
	timer.c \
	variant.c \
	variant_map.c

