SUBDIRS = include src

if ENABLE_TOOLS
SUBDIRS += tools
endif

if ENABLE_TESTS
SUBDIRS += tests
endif

# AM_MAKEFLAGS = -s
ACLOCAL_AMFLAGS = -I m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libeu.pc

EXTRA_DIST = \
	libeu.pc.in \
	include/eu/version.h.in

if HAVE_GCOV
ENABLE_COVERAGE = yes
else
ENABLE_COVERAGE = no
endif

include Makefile.am.coverage
