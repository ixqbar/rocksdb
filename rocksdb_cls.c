/*
 * rocksdb_class.c
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "Zend/zend_types.h"
#include "php_rocksdb.h"
#include "rocksdb_exception.h"
#include "rocksdb_cls.h"
#include <unistd.h>

zend_class_entry *rocksdb_class_ce;
zend_object_handlers rocksdb_object_handlers;

static void rocksdb_object_destroy(rocksdb_class_object_arg *object) {
	ROCKSDB_CLS *obj = (ROCKSDB_CLS *)(object);
	if (obj->zo.properties) {
		zend_hash_destroy(obj->zo.properties);
		FREE_HASHTABLE(obj->zo.properties);
	}
	if (obj->db) {
		rocksdb_close(obj->db);
	}
	zend_object_std_dtor(&(obj->zo));
	efree(obj);
}

static rocksdb_class_create_object_retval rocksdb_create_object(zend_class_entry *class_type) {
	ROCKSDB_CLS *obj;

	obj = ROCKSDB_ALLOC_OBJECT_T(ROCKSDB_CLS, class_type);

	zend_object_std_init((zend_object *)obj, class_type TSRMLS_CC);
	object_properties_init((zend_object*)obj, class_type);

#if PHP_MAJOR_VERSION >= 7
	obj->zo.handlers = &rocksdb_object_handlers;

	return &(obj->zo);
#else
	{
		rocksdb_class_create_object_retval retval;
		retval.handle = zend_objects_store_put(obj,(zend_objects_store_dtor_t) zend_objects_destroy_object, (zend_objects_free_object_storage_t) rocksdb_object_destroy, NULL TSRMLS_CC);
		retval.handlers = &rocksdb_object_handlers;

		return retval;
	}
#endif
}

ZEND_BEGIN_ARG_INFO_EX(rocksdb_class__construct_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(rocksdb_class_get_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(rocksdb_class_set_arginfo, 0, 0, 2)
	ZEND_ARG_INFO(0, key)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(rocksdb_class_delete_arginfo, 0, 0, 1)
	ZEND_ARG_INFO(0, key)
ZEND_END_ARG_INFO()

PHP_METHOD(rocksdb_cls, __construct) {
	char *path = NULL;
	size_t path_len = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &path, &path_len) == FAILURE
			|| path_len == 0) {
		rocksdb_throw_exception(1, "__construct function param must be a folder path");
		return;
	}

	ROCKSDB_CLS *obj = ROCKSDB_FETCH_OBJECT(getThis());
	if (obj == NULL) {
		rocksdb_throw_exception(1, "get rocksdb instance failed");
		return;
	}

	rocksdb_options_t *options = rocksdb_options_create();
	long cpus = sysconf(_SC_NPROCESSORS_ONLN);
	rocksdb_options_increase_parallelism(options, (int)(cpus));
	rocksdb_options_optimize_level_style_compaction(options, 0);
	rocksdb_options_set_create_if_missing(options, 1);

	char *err = NULL;
	rocksdb_t *db = rocksdb_open(options, path, &err);
	if (err) {
		rocksdb_options_destroy(options);
		rocksdb_throw_exception(1, "open rocksdb path failed");
		return;
	}

	obj->db = db;
	rocksdb_options_destroy(options);
}

PHP_METHOD(rocksdb_cls, set) {
	char *k, *v;
	size_t kl = 0, vl = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &k, &kl, &v, &vl) == FAILURE) {
		RETURN_FALSE;
	}

	if (kl == 0) {
		rocksdb_throw_exception(1, "set params invalid");
		RETURN_FALSE;
	}

	ROCKSDB_CLS *obj = ROCKSDB_FETCH_OBJECT(getThis());
	if (obj == NULL) {
		rocksdb_throw_exception(1, "get rocksdb instance failed");
		return;
	}

	char *err = NULL;
	rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
	rocksdb_put(obj->db, writeoptions, k, kl, v, vl, &err);
	rocksdb_writeoptions_destroy(writeoptions);
	if (err != NULL) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_METHOD(rocksdb_cls, get) {
	char *k;
	size_t kl = 0, vl = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &k, &kl) == FAILURE) {
		RETURN_FALSE;
	}

	if (kl == 0) {
		rocksdb_throw_exception(1, "get params invalid");
		RETURN_FALSE;
	}

	ROCKSDB_CLS *obj = ROCKSDB_FETCH_OBJECT(getThis());
	if (obj == NULL) {
		rocksdb_throw_exception(1, "get rocksdb instance failed");
		return;
	}

	char *err = NULL;
	rocksdb_readoptions_t *readoptions = rocksdb_readoptions_create();
	char *v = rocksdb_get(obj->db, readoptions, k, kl, &vl, &err);
	rocksdb_readoptions_destroy(readoptions);
	if (err != NULL) {
		RETURN_NULL();
	}

#if PHP_MAJOR_VERSION >= 7
	RETURN_STRINGL(v, vl);
#else
	RETVAL_STRINGL(v, vl, 1);
#endif
}

PHP_METHOD(rocksdb_cls, delete) {
	char *k;
	size_t kl = 0;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &k, &kl) == FAILURE) {
		RETURN_FALSE;
	}

	if (kl == 0) {
		rocksdb_throw_exception(1, "delete params invalid");
		RETURN_FALSE;
	}

	ROCKSDB_CLS *obj = ROCKSDB_FETCH_OBJECT(getThis());
	if (obj == NULL) {
		rocksdb_throw_exception(1, "get rocksdb instance failed");
		return;
	}

	char *err = NULL;
	rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
	rocksdb_delete(obj->db, writeoptions, k, kl, &err);
	rocksdb_writeoptions_destroy(writeoptions);
	if (err != NULL) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}

PHP_METHOD(rocksdb_cls, close) {
	ROCKSDB_CLS *obj = ROCKSDB_FETCH_OBJECT(getThis());
	if (obj == NULL) {
		rocksdb_throw_exception(1, "get rocksdb instance failed");
		return;
	}

	if (obj->db) {
		rocksdb_close(obj->db);
		obj->db = NULL;
	}
}

zend_function_entry rocksdb_class_methods[] = {
	PHP_ME(rocksdb_cls, __construct, rocksdb_class__construct_arginfo, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(rocksdb_cls, get,         rocksdb_class_get_arginfo,        ZEND_ACC_PUBLIC)
	PHP_ME(rocksdb_cls, set,         rocksdb_class_set_arginfo,        ZEND_ACC_PUBLIC)
	PHP_ME(rocksdb_cls, delete,      rocksdb_class_delete_arginfo,     ZEND_ACC_PUBLIC)
	PHP_ME(rocksdb_cls, close,       NULL,     						   ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};

ROCKSDB_STARTUP_FUNCTION(cls) {
	zend_class_entry ce;

	INIT_CLASS_ENTRY(ce, "RocksDB", rocksdb_class_methods);
	ce.create_object = rocksdb_create_object;
	memcpy(&rocksdb_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	rocksdb_object_handlers.clone_obj = NULL;
#if PHP_MAJOR_VERSION >= 7
	rocksdb_object_handlers.offset   = XtOffsetOf(ROCKSDB_CLS, zo);
	rocksdb_object_handlers.free_obj = rocksdb_object_destroy;
#endif
	rocksdb_class_ce = zend_register_internal_class(&ce);

	return SUCCESS;
}
