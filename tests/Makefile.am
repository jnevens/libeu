AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = -pthread -lcheck_pic -lrt -lm
LDADD = ../src/libbus.la

TESTS = check_old
@VALGRIND_CHECK_RULES@

check_PROGRAMS = $(TESTS)

check_old_SOURCES = check_old.c
