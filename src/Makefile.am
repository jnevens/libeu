AM_CPPFLAGS = -I$(top_buildir)/include -I$(top_srcdir)/include
AM_CFLAGS =

lib_LTLIBRARIES = libbus.la
sbin_PROGRAMS = busd client

libbus_la_LIBADD =
libbus_la_LDFLAGS =

libbus_la_SOURCES = \
	event.c \
	list.c \
	log.c \
	object.c \
	parameter.c \
	string.c \
	timer.c \
	variant.c \
	daemon.c \
	socket.c

busd_LDADD = $(top_builddir)/src/.libs/libbus.la
busd_SOURCES = \
	busd.c
	
client_LDADD = $(top_builddir)/src/.libs/libbus.la
client_SOURCES = \
	client.c
