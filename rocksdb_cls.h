/*
 * rocksdb_class.h
 *
 *  Created on: Dec 9, 2015
 *      Author: xingqiba
 */

#ifndef EXT_ROCKSDB_ROCKSDB_CLASS_H_
#define EXT_ROCKSDB_ROCKSDB_CLASS_H_

#include "php_rocksdb.h"
#include "rocksdb/c.h"

extern zend_class_entry *rocksdb_class_ce;
extern zend_object_handlers rocksdb_object_handlers;

typedef struct {
	zend_object zo;
	rocksdb_t *db;
} ROCKSDB_CLS;

#if PHP_MAJOR_VERSION >= 7
#define rocksdb_class_create_object_retval zend_object*
#define rocksdb_class_object_arg zend_object
#define ROCKSDB_ALLOC_OBJECT_T(_obj_t, _class_type) (_obj_t *)ecalloc(1, sizeof(_obj_t)+zend_object_properties_size(_class_type))
#define ROCKSDB_FETCH_OBJECT(obj) (ROCKSDB_CLS *)Z_OBJ_P(obj);
#else
#define rocksdb_class_create_object_retval zend_object_value
#define rocksdb_class_object_arg void
#define ROCKSDB_ALLOC_OBJECT_T(_obj_t, _class_type) (_obj_t *)ecalloc(1, sizeof(_obj_t))
#define ROCKSDB_FETCH_OBJECT(obj) (ROCKSDB_CLS *)zend_object_store_get_object(obj)
#endif


ROCKSDB_STARTUP_FUNCTION(cls);

#endif /* EXT_ROCKSDB_ROCKSDB_CLASS_H_ */
