AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_srcdir)/include
AM_CFLAGS = -D_GNU_SOURCE

lib_LTLIBRARIES = libeu.la

libeu_la_LIBADD = -ljson-c
libeu_la_LDFLAGS = -version-info $(DC_VERSION_LIBTOOL)

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
	variant.c \
	variant_map.c \
	version.c

$(libeu_la_OBJECTS): $(abs_top_builddir)/include/eu/version.h

