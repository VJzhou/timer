<?php

// (new Task())->test();

// class Task  {
//
//     protected $seconds = 0;
//
//     public function __construct ($sec = 0) {
//         $this->$seconds = $sec;
//     }
//
//     public function run (Callable $func) {
//
//     }
//
// }

class Test{
    protected static $sec = 0;
    public function __construct (int $sec = 0) {
        self::$sec = $sec;
    }

    public static function run () {
        echo self::$sec;
    }
}

function test ($a, $b) {
     echo $a .PHP_EOL;
     echo $b .PHP_EOL;
     (new Test)->run();
}

// Task::run('test', 'a', 'b');
// (new Task(2))->test();
// $a = 2;
// (new Task($a))::run('test', 'a', 'b');

$t = new Task(2);
$t::run('test', 'a', 'b');

// (new Test(1))::run();
// (new Test)->run();