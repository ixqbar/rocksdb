/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_ROCKSDB_H
#define PHP_ROCKSDB_H

extern zend_module_entry rocksdb_module_entry;
#define phpext_rocksdb_ptr &rocksdb_module_entry

#define PHP_ROCKSDB_VERSION "0.0.1"

#define ROCKSDB_STARTUP_FUNCTION(module)   	ZEND_MINIT_FUNCTION(rocksdb_##module)
#define ROCKSDB_RINIT_FUNCTION(module)		ZEND_RINIT_FUNCTION(rocksdb_##module)
#define ROCKSDB_STARTUP(module)	 		  	ZEND_MODULE_STARTUP_N(rocksdb_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define ROCKSDB_SHUTDOWN_FUNCTION(module)  	ZEND_MSHUTDOWN_FUNCTION(rocksdb_##module)
#define ROCKSDB_SHUTDOWN(module)	 	    ZEND_MODULE_SHUTDOWN_N(rocksdb_##module)(INIT_FUNC_ARGS_PASSTHRU)

#ifdef PHP_WIN32
#	define PHP_ROCKSDB_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_ROCKSDB_API __attribute__ ((visibility("default")))
#else
#	define PHP_ROCKSDB_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(rocksdb);
PHP_MSHUTDOWN_FUNCTION(rocksdb);
PHP_RINIT_FUNCTION(rocksdb);
PHP_RSHUTDOWN_FUNCTION(rocksdb);
PHP_MINFO_FUNCTION(rocksdb);

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(rocksdb)
	zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(rocksdb)
*/

/* Always refer to the globals in your function as ROCKSDB_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define ROCKSDB_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(rocksdb, v)

#if defined(ZTS) && defined(COMPILE_DL_ROCKSDB)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_ROCKSDB_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
