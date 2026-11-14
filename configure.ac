# Versioning.
m4_define([dc_version_major],[0])
m4_define([dc_version_minor],[0])
m4_define([dc_version_micro],[2])
m4_define([dc_version_suffix],[devel])
m4_define([dc_version],dc_version_major.dc_version_minor.dc_version_micro[]m4_ifset([dc_version_suffix],-[dc_version_suffix])) 
# Libtool versioning.
m4_define([dc_version_lt_current],[0])
m4_define([dc_version_lt_revision],[0])
m4_define([dc_version_lt_age],[0])

# Initialize autoconf
AC_PREREQ([2.60])
AC_INIT([libeu],[dc_version])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PREFIX_DEFAULT([/usr])
# AC_PROG_RANLIB

# Initialize automake.
AM_INIT_AUTOMAKE([-Wall -Werror -Wno-portability foreign])

# Fix for automake >= 1.12
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Initialize libtool.
LT_PREREQ([2.2.0])
LT_INIT()
LT_PROG_RC

# Tool applications.
AC_ARG_ENABLE([tools],
        [AS_HELP_STRING([--enable-tools=@<:@yes/no@:>@],
                [Build tools @<:@default=yes@:>@])],
        [], [enable_tools=yes])
AM_CONDITIONAL([ENABLE_TOOLS], [test "x$enable_tools" = "xyes"])

# Tests. Accept "y" as well as "yes" (--enable-tests=y).
AC_ARG_ENABLE([tests],
	[AS_HELP_STRING([--enable-tests=@<:@yes/no@:>@],
		[Build self tests @<:@default=no@:>@])],
	[], [enable_tests=no])
AS_CASE([$enable_tests],
	[yes|y], [enable_tests=yes],
	[enable_tests=no])

AX_VALGRIND_CHECK

AM_CONDITIONAL([ENABLE_LIBEVQUICK], [test "x$with_libevquick" = "xyes"])

# Checks for programs.
AC_PROG_CC

# gcov coverage reporting

m4_include([m4/gcov.m4])
AC_TDD_GCOV
AC_SUBST(COVERAGE_CFLAGS)
AC_SUBST(COVERAGE_CXXFLAGS)
AC_SUBST(COVERAGE_LDFLAGS)

# Coverage replays the test suite, so turn tests on with gcov.
AS_IF([test "x$use_gcov" = "xyes" && test "x$enable_tests" != "xyes"],
	[enable_tests=yes])
AM_CONDITIONAL([ENABLE_TESTS], [test "x$enable_tests" = "xyes"])
AS_IF([test "x$enable_tests" = "xyes"], [
	PKG_CHECK_MODULES([CHECK], [check])
	# libcheck is a static archive that needs libsubunit. Link the
	# subunit archive itself so the tests do not depend on
	# libsubunit.so.0 being on the default runtime path.
	subunit_libdir=`$PKG_CONFIG --variable=libdir libsubunit`
	AS_IF([test -n "$subunit_libdir" && test -f "$subunit_libdir/libsubunit.a"], [
		CHECK_LIBS=`echo "$CHECK_LIBS" | sed "s|-lsubunit|$subunit_libdir/libsubunit.a|"`
	])
])

# Versioning.
AC_SUBST([DC_VERSION],[dc_version])
AC_SUBST([DC_VERSION_MAJOR],[dc_version_major])
AC_SUBST([DC_VERSION_MINOR],[dc_version_minor])
AC_SUBST([DC_VERSION_MICRO],[dc_version_micro])
AC_SUBST([DC_VERSION_LIBTOOL],[dc_version_lt_current:dc_version_lt_revision:dc_version_lt_age])

# Version suffix.
m4_ifset([dc_version_suffix],[
        AC_DEFINE(HAVE_VERSION_SUFFIX, [1], [Define if a version suffix is p    resent.])
])

AC_CONFIG_FILES([
	libeu.pc
	Makefile
	include/Makefile
	include/eu/Makefile
	src/Makefile
	tests/Makefile
	tools/Makefile
])

AC_OUTPUT
