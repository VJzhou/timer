--TEST--
test1() Basic test
--EXTENSIONS--
timer
--FILE--
<?php
$ret = test1();

var_dump($ret);
?>
--EXPECT--
The extension timer is loaded and working!
NULL
