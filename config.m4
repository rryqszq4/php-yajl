dnl $Id$
dnl config.m4 for extension yajl

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(yajl, for yajl support,
dnl Make sure that the comment is aligned:
dnl [  --with-yajl             Include yajl support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(yajl, whether to enable yajl support,
Make sure that the comment is aligned:
[  --enable-yajl           Enable yajl support])

if test "$PHP_YAJL" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-yajl -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/yajl.h"  # you most likely want to change this
  dnl if test -r $PHP_YAJL/$SEARCH_FOR; then # path given as parameter
  dnl   YAJL_DIR=$PHP_YAJL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for yajl files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       YAJL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$YAJL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the yajl distribution])
  dnl fi

  dnl # --with-yajl -> add include path
  dnl PHP_ADD_INCLUDE($YAJL_DIR/include)

  dnl # --with-yajl -> check for lib and symbol presence
  dnl LIBNAME=yajl # you may want to change this
  dnl LIBSYMBOL=yajl # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YAJL_DIR/lib, YAJL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_YAJLLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong yajl lib version or lib not found])
  dnl ],[
  dnl   -L$YAJL_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(YAJL_SHARED_LIBADD)

  YAJL_SOURCES="\
    yajl/yajl_version.c \
    yajl/yajl.c \
    yajl/yajl_encode.c \
    yajl/yajl_lex.c \
    yajl/yajl_parser.c \
    yajl/yajl_buf.c \
    yajl/yajl_tree.c \
    yajl/yajl_alloc.c \
    yajl/yajl_gen.c"

  PHP_NEW_EXTENSION(yajl, yajl.c $YAJL_SOURCES, $ext_shared)
fi
