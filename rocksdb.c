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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_rocksdb.h"

#include "rocksdb_exception.h"
#include "rocksdb_cls.h"

/* If you declare any globals in php_rocksdb.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(rocksdb)
*/

/* True global resources - no need for thread safety here */
static int le_rocksdb;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("rocksdb.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_rocksdb_globals, rocksdb_globals)
    STD_PHP_INI_ENTRY("rocksdb.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_rocksdb_globals, rocksdb_globals)
PHP_INI_END()
*/
/* }}} */

PHP_FUNCTION(rocksdb_version)
{
#if PHP_MAJOR_VERSION >=7
	RETURN_STRING(PHP_ROCKSDB_VERSION);
#else
	RETURN_STRING(PHP_ROCKSDB_VERSION, 1);
#endif
}


/* {{{ php_rocksdb_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_rocksdb_init_globals(zend_rocksdb_globals *rocksdb_globals)
{
	rocksdb_globals->global_value = 0;
	rocksdb_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(rocksdb)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

	/* startup components */
	ROCKSDB_STARTUP(exception);
	ROCKSDB_STARTUP(cls);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(rocksdb)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(rocksdb)
{
#if defined(COMPILE_DL_ROCKSDB) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(rocksdb)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(rocksdb)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "rocksdb support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ rocksdb_functions[]
 *
 * Every user visible function must have an entry in rocksdb_functions[].
 */
const zend_function_entry rocksdb_functions[] = {
	PHP_FE(rocksdb_version,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in rocksdb_functions[] */
};
/* }}} */

/* {{{ rocksdb_module_entry
 */
zend_module_entry rocksdb_module_entry = {
	STANDARD_MODULE_HEADER,
	"rocksdb",
	rocksdb_functions,
	PHP_MINIT(rocksdb),
	PHP_MSHUTDOWN(rocksdb),
	PHP_RINIT(rocksdb),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(rocksdb),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(rocksdb),
	PHP_ROCKSDB_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ROCKSDB
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(rocksdb)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
