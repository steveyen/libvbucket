AC_PREREQ(2.52)
m4_include([m4/version.m4])
m4_include([m4/c99-backport.m4])
AC_INIT(libvbucket, VERSION_NUMBER, sean@northscale.com)
AC_CANONICAL_SYSTEM
AC_CONFIG_SRCDIR(vbucket.c)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

if test "x${enable_dependency_tracking}" = "x"
then
  enable_dependency_tracking=yes
fi

dnl **********************************************************************
dnl DETECT_ICC ([ACTION-IF-YES], [ACTION-IF-NO])
dnl
dnl check if this is the Intel ICC compiler, and if so run the ACTION-IF-YES
dnl sets the $ICC variable to "yes" or "no"
dnl **********************************************************************
AC_DEFUN([DETECT_ICC],
[
    ICC="no"
    AC_MSG_CHECKING([for icc in use])
    if test "$GCC" = "yes"; then
       dnl check if this is icc acting as gcc in disguise
       AC_EGREP_CPP([^__INTEL_COMPILER], [__INTEL_COMPILER],
         AC_MSG_RESULT([no])
         [$2],
         AC_MSG_RESULT([yes])
         [$1]
         ICC="yes")
    else
       AC_MSG_RESULT([no])
       [$2]
    fi
])

dnl **********************************************************************
dnl DETECT_SUNCC ([ACTION-IF-YES], [ACTION-IF-NO])
dnl
dnl check if this is the Sun Studio compiler, and if so run the ACTION-IF-YES
dnl sets the $SUNCC variable to "yes" or "no"
dnl **********************************************************************
AC_DEFUN([DETECT_SUNCC],
[
    SUNCC="no"
    AC_MSG_CHECKING([for Sun cc in use])
    AC_RUN_IFELSE(
      [AC_LANG_PROGRAM([], [dnl
#ifdef __SUNPRO_C
   return 0;
#else
   return 1;
#endif
      ])
    ],[
       AC_MSG_RESULT([yes])
       [$1]
       SUNCC="yes"
    ], [
       AC_MSG_RESULT([no])
       [$2]
    ])
])

AS_IF(test "x$GCC" = "xyes",
      DETECT_ICC([CFLAGS="-pthread $CFLAGS"], [CFLAGS="-fvisibility=hidden -pthread $CFLAGS"]),
      ICC=no
      DETECT_SUNCC([CFLAGS="-xldscope=hidden -mt $CFLAGS"], []))

if test "$ICC" = "no"; then
   AC_PROG_CC_C99
fi

AM_PROG_CC_C_O
AC_PROG_INSTALL

AC_SUBST(PROFILER_LDFLAGS)

AC_ARG_ENABLE(coverage,
  [AS_HELP_STRING([--enable-coverage],[Disable code coverage])])

if test "x$enable_coverage" = "xyes"; then
   if test "$ICC" = "yes"
   then
      AC_MSG_WARN([icc doesn't support code coverage checking])
   elif test "$GCC" = "yes"
   then
      AC_PATH_PROG([PROFILER], [gcov], "no", [$PATH])
      if test "x$PROFILER" != "xno"; then
         # Issue 97: The existense of gcov doesn't mean we have -lgcov
         AC_CHECK_LIB(gcov, main,
                    [
                      PROFILER_FLAGS="-fprofile-arcs -ftest-coverage"
                      PROFILER_LDFLAGS="-lgcov"
                    ], [
                      PROFILER_FLAGS=
                      PROFILER_LDFLAGS=
                    ])
      fi
   elif test "$SUNCC" = "yes"
   then
      AC_PATH_PROG([PROFILER], [tcov], "no", [$PATH])
      if test "x$PROFILER" != "xno"; then
         PROFILER_FLAGS=-xprofile=tcov
      fi
   fi
fi
AC_SUBST(PROFILER_FLAGS)


AC_ARG_ENABLE(64bit,
  [AS_HELP_STRING([--enable-64bit],[build 64bit version])])
if test "x$enable_64bit" = "xyes"
then
    org_cflags=$CFLAGS
    CFLAGS=-m64
    AC_RUN_IFELSE(
      [AC_LANG_PROGRAM([], [dnl
return sizeof(void*) == 8 ? 0 : 1;
      ])
    ],[
      CFLAGS="-m64 $org_cflags"
    ],[
    AC_MSG_ERROR([Don't know how to build a 64-bit object.])
    ])
fi

AC_HEADER_STDBOOL
AH_BOTTOM([#if HAVE_STDBOOL_H
#include <stdbool.h>
#else
#define bool char
#define false 0
#define true 1
#endif ])

AC_CHECK_HEADERS([inttypes.h])
AH_BOTTOM([#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
])

dnl **********************************************************************
dnl DETECT_UINT64_SUPPORT
dnl
dnl check if we can use a uint64_t
dnl **********************************************************************
AC_DEFUN([AC_C_DETECT_UINT64_SUPPORT],
[
    AC_CACHE_CHECK([for print macros for integers (C99 section 7.8.1)],
        [ac_cv_c_uint64_support],
        [AC_TRY_COMPILE(
            [
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#include <stdio.h>
            ], [
  uint64_t val = 0;
  fprintf(stderr, "%" PRIu64 "\n", val);
            ],
            [ ac_cv_c_uint64_support=yes ],
            [ ac_cv_c_uint64_support=no ])
        ])
])

AC_C_DETECT_UINT64_SUPPORT
AS_IF([test "x$ac_cv_c_uint64_support" = "xno"],
      [AC_MSG_WARN([

Failed to use print macros (PRIu) as defined in C99 section 7.8.1.

])])

AC_C_CONST

dnl From licq: Copyright (c) 2000 Dirk Mueller
dnl Check if the type socklen_t is defined anywhere
AC_DEFUN([AC_C_SOCKLEN_T],
[AC_CACHE_CHECK(for socklen_t, ac_cv_c_socklen_t,
[
  AC_TRY_COMPILE([
    #include <sys/types.h>
    #include <sys/socket.h>
  ],[
    socklen_t foo;
  ],[
    ac_cv_c_socklen_t=yes
  ],[
    ac_cv_c_socklen_t=no
  ])
])
if test $ac_cv_c_socklen_t = no; then
  AC_DEFINE(socklen_t, int, [define to int if socklen_t not available])
fi
])

AC_C_SOCKLEN_T

dnl Check if we're a little-endian or a big-endian system, needed by hash code
AC_DEFUN([AC_C_ENDIAN],
[AC_CACHE_CHECK(for endianness, ac_cv_c_endian,
[
  AC_RUN_IFELSE(
    [AC_LANG_PROGRAM([], [dnl
        long val = 1;
        char *c = (char *) &val;
        exit(*c == 1);
    ])
  ],[
    ac_cv_c_endian=big
  ],[
    ac_cv_c_endian=little
  ])
])
if test $ac_cv_c_endian = big; then
  AC_DEFINE(ENDIAN_BIG, 1, [machine is bigendian])
fi
if test $ac_cv_c_endian = little; then
  AC_DEFINE(ENDIAN_LITTLE, 1, [machine is littleendian])
fi
])

AC_C_ENDIAN

AC_DEFUN([AC_C_HTONLL],
[
    AC_MSG_CHECKING([for htonll])
    have_htoll="no"
    AC_RUN_IFELSE([
       AC_LANG_PROGRAM([
#include <sys/types.h>
#include <netinet/in.h>
#ifdef HAVE_INTTYPES_H
#include <inttypes.h> */
#endif
       ], [
          return htonll(0);
       ])
    ], [
      have_htoll="yes"
      AC_DEFINE([HAVE_HTONLL], [1], [Have ntohll])
    ])

    AC_MSG_RESULT([$have_htoll])
])

AC_C_HTONLL

dnl Check whether the user's system supports pthread
AC_SEARCH_LIBS(pthread_create, pthread)
if test "x$ac_cv_search_pthread_create" = "xno"; then
  AC_MSG_ERROR([Can't enable threads without the POSIX thread library.])
fi

AC_CHECK_FUNCS(mlockall)
AC_CHECK_FUNCS(getpagesizes)
AC_CHECK_FUNCS(memcntl)
AC_CHECK_FUNCS(sigignore)

AC_DEFUN([AC_C_ALIGNMENT],
[AC_CACHE_CHECK(for alignment, ac_cv_c_alignment,
[
  AC_RUN_IFELSE(
    [AC_LANG_PROGRAM([
#include <stdlib.h>
#include <inttypes.h>
    ], [
       char *buf = malloc(32);

       uint64_t *ptr = (uint64_t*)(buf+2);
       // catch sigbus, etc.
       *ptr = 0x1;

       // catch unaligned word access (ARM cpus)
       *buf =  1; *(buf +1) = 2; *(buf + 2) = 2; *(buf + 3) = 3; *(buf + 4) = 4;
       int* i = (int*)(buf+1);
       return (84148994 == i) ? 0 : 1;
    ])
  ],[
    ac_cv_c_alignment=none
  ],[
    ac_cv_c_alignment=need
  ])
])
if test $ac_cv_c_alignment = need; then
  AC_DEFINE(NEED_ALIGN, 1, [Machine need alignment])
fi
])

AC_C_ALIGNMENT

dnl Check for the requirements for running memcached with less privileges
dnl than the default privilege set. On Solaris we need setppriv and priv.h
dnl If you want to add support for other platforms you should check for
dnl your requirements, define HAVE_DROP_PRIVILEGES, and make sure you add
dnl the source file containing the implementation into memcached_SOURCE
dnl in Makefile.am
AC_CHECK_FUNCS(setppriv, [
   AC_CHECK_HEADER(priv.h, [
      AC_DEFINE([HAVE_DROP_PRIVILEGES], 1,
         [Define this if you have an implementation of drop_privileges()])
      build_solaris_privs=yes
   ], [])
],[])

AM_CONDITIONAL([BUILD_SOLARIS_PRIVS],[test "$build_solaris_privs" = "yes"])

AC_CHECK_HEADER(umem.h, [
   AC_DEFINE([HAVE_UMEM_H], 1,
         [Define this if you have umem.h])
   build_cache=no
], [build_cache=yes])

AM_CONDITIONAL([BUILD_CACHE], [test "x$build_cache" = "xyes"])

dnl Don't generate text files from the XML documents by default.
dnl Most developers just run ./configure and will run a lot of make / make clean
dnl and we don't want them to spend their time waiting for doc generation ;-)
dnl If you really want the documents you should be capable of adding
dnl --enable-docs yourself...
AC_ARG_ENABLE(docs,
  [AS_HELP_STRING([--enable-docs],[Disable documentation generation])])

if test "x$enable_docs" = "xyes"; then
  AC_PATH_PROG([XML2RFC], [xml2rfc], "no")
  if test "x$XML2RFC" = "xno"; then
    AC_MSG_ERROR([Can't build documentation without xml2rfc.])
  fi
  AC_PATH_PROG([XSLTPROC], [xsltproc], "no")
  if test "x$XSLTPROC" = "xno"; then
    AC_MSG_ERROR([Can't build documentation without xsltproc.])
  fi
fi
AM_CONDITIONAL([BUILD_SPECIFICATIONS], [test "x$enable_docs" = "xyes"])

dnl Let the compiler be a bit more picky. Please note that you cannot
dnl specify these flags to the compiler before AC_CHECK_FUNCS, because
dnl the test program will generate a compilation warning and hence fail
dnl to detect the function ;-)
if test "$ICC" = "yes"
then
   dnl ICC trying to be gcc.
   CFLAGS="$CFLAGS -diag-disable 187 -Wall -Werror"
   AC_DEFINE([_GNU_SOURCE],[1],[find sigignore on Linux])
elif test "$GCC" = "yes"
then
  GCC_VERSION=`$CC -dumpversion`
  CFLAGS="$CFLAGS -Wall -Werror -pedantic -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls"
  case $GCC_VERSION in
    4.4.*)
    CFLAGS="$CFLAGS -fno-strict-aliasing"
    ;;
  esac
  AC_DEFINE([_GNU_SOURCE],[1],[find sigignore on Linux])
elif test "$SUNCC" = "yes"
then
  CFLAGS="$CFLAGS -errfmt=error -errwarn -errshort=tags"
fi

dnl Check for stuff needed to run Doxygen
AC_PATH_PROG([DOT], [dot], "no")
AS_IF(test "x$DOT" = "xno", [HAVE_DOT=NO], [HAVE_DOT=YES])
AC_SUBST(HAVE_DOT)
PROJECT_NUMBER=VERSION_NUMBER
AC_SUBST(PROJECT_NUMBER)

dnl Generate output files
CPPFLAGS="-I\${top_srcdir}/include ${CPPFLAGS}"

trylibmemcacheddir=""
AC_ARG_WITH(libmemcached,
       [  --with-libmemcached=PATH     Specify path to libmemcached installation ],
       [
                if test "x$withval" != "xno" ; then
                        trylibmemcacheddir=$withval
                fi
       ]
)

dnl ----------------------------------------------------------------------------

AC_CACHE_CHECK([for libmemcached directory], ac_cv_libmemcached_dir, [
  saved_CPPFLAGS="$CPPFLAGS"
  lmc_found=no
  for lmcdir in $trylibmemcacheddir "" $prefix /usr/local ; do
    # Skip the directory if it isn't there.
    if test ! -z "$lmcdir" -a ! -d "$lmcdir" ; then
       continue;
    fi
    if test ! -z "$lmcdir" ; then
      if test -d "$lmcdir/include" ; then
        CPPFLAGS="-I$lmcdir $CPPFLAGS"
      else
        CPPFLAGS="-I$lmcdir $CPPFLAGS"
      fi
    fi
    # Can I compile and link it?
    AC_TRY_LINK([#include <libmemcached/memcached.h>], [ ],
       [ libmemcached_linked=yes ], [ libmemcached_linked=no ])
    if test $libmemcached_linked = yes; then
       if test ! -z "$lmcdir" ; then
         ac_cv_libmemcached_dir=$lmcdir
       else
         ac_cv_libmemcached_dir="(system)"
       fi
       lmc_found=yes
       break
    fi
  done
  CPPFLAGS="$saved_CPPFLAGS"
  if test $lmc_found = no ; then
    AC_MSG_ERROR([libmemcached is required.

      If it's already installed, specify its path using --with-libmemcached=/dir/
])
  fi
])
if test $ac_cv_libmemcached_dir != "(system)"; then
  if test -d "$ac_cv_libmemcached_dir/lib" ; then
    LDFLAGS="-L$ac_cv_libmemcached_dir/lib $LDFLAGS"
    mc_libdir="$ac_cv_libmemcached_dir/lib"
  else
    LDFLAGS="-L$ac_cv_libmemcached_dir $LDFLAGS"
    mc_libdir="$ac_cv_libmemcached_dir"
  fi
  if test -d "$ac_cv_libmemcached_dir/include" ; then
    CPPFLAGS="-I$ac_cv_libmemcached_dir/include $CPPFLAGS"
  else
    CPPFLAGS="-I$ac_cv_libmemcached_dir $CPPFLAGS"
  fi
fi

LIBMEMCACHED_DIR=$ac_cv_libmemcached_dir
AS_IF([test "${LIBMEMCACHED_DIR}" = "(system)"],
      [
      AS_IF([test "x$ac_cv_have_htonll" = "xyes"],[ LIBMEMCACHED_DIR=. ], [
        AC_MSG_ERROR([
ep-engine needs to link with a utility lib from libmemcached.
Specify its path using --with-libmemcached. ex: --with-libmemcached=../libmemcached
])
        ])
      ])
AC_SUBST(LIBMEMCACHED_DIR)

dnl ----------------------------------------------------------------------------

AC_CONFIG_FILES(Makefile)
AC_OUTPUT

