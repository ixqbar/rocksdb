dnl $Id$
dnl config.m4 for extension rocksdb

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(rocksdb, for rocksdb support,
Make sure that the comment is aligned:
[  --with-rocksdb             Include rocksdb support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(rocksdb, whether to enable rocksdb support,
Make sure that the comment is aligned:
[  --enable-rocksdb           Enable rocksdb support])

if test "$PHP_ROCKSDB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-rocksdb -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/rocksdb/c.h"  # you most likely want to change this
  if test -r $PHP_ROCKSDB/$SEARCH_FOR; then # path given as parameter
        ROCKSDB_DIR=$PHP_ROCKSDB
  else # search default path list
        AC_MSG_CHECKING([for rocksdb files in default path])
        for i in $SEARCH_PATH ; do
          if test -r $i/$SEARCH_FOR; then
            ROCKSDB_DIR=$i
            AC_MSG_RESULT(found in $i)
          fi
        done
  fi
  
  if test -z "$ROCKSDB_DIR"; then
     AC_MSG_RESULT([not found])
     AC_MSG_ERROR([Please reinstall the rocksdb distribution])
  else
     AC_MSG_RESULT([Found rocksdb header in $ROCKSDB_DIR/include])
  fi

  dnl # --with-rocksdb -> add include path
  PHP_ADD_INCLUDE($ROCKSDB_DIR/include)

  dnl # --with-rocksdb -> check for lib and symbol presence
  LIBNAME=rocksdb
  LIBSYMBOL=rocksdb_open
  
  PHP_REQUIRE_CXX()
  PHP_ADD_LIBRARY(stdc++, 1, ROCKSDB_SHARED_LIBADD)

  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
    [
        PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ROCKSDB_DIR/$PHP_LIBDIR, ROCKSDB_SHARED_LIBADD)
        AC_DEFINE(HAVE_ROCKSDBLIB,1,[ ])
    ],[
        AC_MSG_ERROR([wrong rocksdb lib version or lib not found])
    ],[
      -L$ROCKSDB_DIR/$PHP_LIBDIR -lm
    ])
  
  PHP_SUBST(ROCKSDB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(rocksdb, 
        rocksdb_exception.c \
        rocksdb_cls.c \
        rocksdb.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
