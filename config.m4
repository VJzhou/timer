dnl config.m4 for extension timer

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([timer],
dnl   [for timer support],
dnl   [AS_HELP_STRING([--with-timer],
dnl     [Include timer support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([timer],
  [whether to enable timer support],
  [AS_HELP_STRING([--enable-timer],
    [Enable timer support])],
  [no])

if test "$PHP_TIMER" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, TIMER_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-timer -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/timer.h"  # you most likely want to change this
  dnl if test -r $PHP_TIMER/$SEARCH_FOR; then # path given as parameter
  dnl   TIMER_DIR=$PHP_TIMER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for timer files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TIMER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TIMER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the timer distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-timer -> add include path
  dnl PHP_ADD_INCLUDE($TIMER_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-timer -> check for lib and symbol presence
  dnl LIBNAME=TIMER # you may want to change this
  dnl LIBSYMBOL=TIMER # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_TIMER_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your timer library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TIMER_DIR/$PHP_LIBDIR, TIMER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TIMER_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your timer library.])
  dnl ],[
  dnl   -L$TIMER_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TIMER_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_TIMER, 1, [ Have timer support ])

  PHP_NEW_EXTENSION(timer,
    timer.c \
    task.c ,
  $ext_shared)

  dnl PHP_INSTALL_HEADERS([ext/timer], [*.h config.h])

fi

dnl if test -z "$PHP_DEBUG"; then
dnl    AC_ARG_ENABLE(debug,
dnl    [ --enable-debug      compile with debugging symbols],[
dnl        PHP_DEBUG=$enableval
dnl    ],[ PHP_DEBUG=no
dnl    ])
dnl fi
