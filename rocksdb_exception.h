/*
 * rocksdb_exception.h
 *
 *  Created on: Dec 9, 2015
 *      Author: xingqiba
 */

#ifndef EXT_ROCKSDB_ROCKSDB_EXCEPTION_H_
#define EXT_ROCKSDB_ROCKSDB_EXCEPTION_H_

void rocksdb_throw_exception(long code, char *message);

ROCKSDB_STARTUP_FUNCTION(exception);

#endif /* EXT_ROCKSDB_ROCKSDB_EXCEPTION_H_ */
