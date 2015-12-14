/*
 * rocksdb_exception.c
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "Zend/zend_exceptions.h"

#include "php_rocksdb.h"
#include "rocksdb_exception.h"

zend_class_entry *rocksdb_exception_ce;

void rocksdb_throw_exception(long code, char *message) {
	zend_throw_exception(rocksdb_exception_ce, message, code);
}

zend_class_entry * rocksdb_get_exception_base(int root) {
#if can_handle_soft_dependency_on_SPL && defined(HAVE_SPL) && ((PHP_MAJOR_VERSION > 5) || (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION >= 1))
	if (!root) {
		if (!spl_ce_RuntimeException) {
			zend_class_entry **pce;

			if (zend_hash_find(CG(class_table), "runtimeexception", sizeof("RuntimeException"), (void **) &pce) == SUCCESS) {
				spl_ce_RuntimeException = *pce;
				return *pce;
			}
		} else {
			return spl_ce_RuntimeException;
		}
	}
#endif

	return zend_exception_get_default();
}

ROCKSDB_STARTUP_FUNCTION(exception) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "RocksDBException", NULL);
#if PHP_MAJOR_VERSION >= 7
	rocksdb_exception_ce = zend_register_internal_class_ex(&ce, rocksdb_get_exception_base(0));
#else
	rocksdb_exception_ce = zend_register_internal_class_ex(&ce, rocksdb_get_exception_base(0), NULL TSRMLS_CC);
#endif
	zend_declare_property_null(rocksdb_exception_ce, ZEND_STRL("message"), 	ZEND_ACC_PROTECTED);
	zend_declare_property_long(rocksdb_exception_ce, ZEND_STRL("code"), 0,  ZEND_ACC_PROTECTED);
	zend_declare_property_null(rocksdb_exception_ce, ZEND_STRL("previous"), ZEND_ACC_PROTECTED);

	return SUCCESS;
}
