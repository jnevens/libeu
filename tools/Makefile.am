AM_CPPFLAGS = -I$(top_buildir)/include -I$(top_srcdir)/include
LDADD = $(top_builddir)/src/.libs/libeu.la  -ljson-c

sbin_PROGRAMS = eubd eubctl eubdummy

eubd_SOURCES = \
	eubd.c
	
eubctl_SOURCES = \
	eubctl.c

eubdummy_SOURCES = \
	eubdummy.c