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
	include/eu/version.h.in \
	Makefile.am.coverage \
	configure.ac \
	m4/gcov.m4 \
	m4/valgrind.m4 \
	misc/cppcheck-env.h \
	misc/cppcheck-suppressions.txt \
	scripts/cppcheck.sh \
	scripts/ci-ssh-key-file.sh \
	scripts/publish-apt-reprepro.sh

if HAVE_GCOV
ENABLE_COVERAGE = yes
else
ENABLE_COVERAGE = no
endif

include Makefile.am.coverage
