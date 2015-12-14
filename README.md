# rocksdb
php extension

##install rocksdb lib
https://github.com/facebook/rocksdb/blob/master/INSTALL.md

##install extension
```
phpize
./configure
make
make install

;add to your php.ini
extension=rocksdb.so
```

##example
```php
$rocksdb_handle = new RocksDB('./test');
$result = $rocksdb_handle->set('name', 'xingqiba');
var_dump($result);

$result = $rocksdb_handle->get('name');
var_dump($result);

$result = $rocksdb_handle->delete('name');
var_dump($result);

$rocksdb_handle->close();
```

###contact
更多疑问请+qq群 233415606 or [website http://xingqiba.sinaapp.com](http://xingqiba.sinaapp.com)