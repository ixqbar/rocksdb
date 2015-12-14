<?php

$rocksdb_handle = new RocksDB('./test');
$result = $rocksdb_handle->set('name', 'xingqiba');
var_dump($result);

$result = $rocksdb_handle->get('name');
var_dump($result);

$result = $rocksdb_handle->delete('name');
var_dump($result);

$rocksdb_handle->close();
