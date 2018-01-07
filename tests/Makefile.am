AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CFLAGS = -pthread -lcheck_pic -lrt -lm -lsubunit
LDADD = ../src/libeu.la

TESTS = check_string check_list check_object check_parameter check_variant check_variant_map
@VALGRIND_CHECK_RULES@

check_PROGRAMS = $(TESTS)

check_string_SOURCES = check_string.c
check_list_SOURCES = check_list.c
check_object_SOURCES = check_object.c
check_parameter_SOURCES = check_parameter.c
check_variant_SOURCES = check_variant.c
check_variant_map_SOURCES = check_variant_map.c
