# Please list files alphabetically in the lists to reduce the risk for
# a merge conflict.
#
bin_PROGRAMS =
noinst_PROGRAMS = testapp
pkginclude_HEADERS = \
                     vbucket.h

noinst_LTLIBRARIES = libvbucket.la

check_PROGRAMS= testapp
TESTS = ${check_PROGRAMS}

# Test application to test stuff from C
testapp_SOURCES = testapp.c
testapp_DEPENDENCIES= libvbucket.la
testapp_LDADD= libvbucket.la

# A collection of functions used by the various modules in memcached
libvbucket_la_SOURCES= \
                        cJSON.c \
                        cJSON.h \
                        vbucket.c \
                        vbucket.h
libvbucket_la_LDFLAGS=
libvbucket_la_LIBADD= @LIBMEMCACHED_DIR@/libhashkit/libhashkit.la


CLEANFILES=
BUILT_SOURCES=

SUBDIRS =
DIST_DIRS =
EXTRA_DIST = m4/version.m4

MOSTLYCLEANFILES = *.gcov *.gcno *.gcda *.tcov

test: check-TESTS

