<?php

// (new Task())->test();

// class Task  {
//
//     protected const TIME_UNIT = [
//         "sec",
//         "min",
//         "hour"
//         "day"
//     ]
//
//     protected const  TYPE_SEC = 0;
//     protected const  TYPE_MIN = 1;
//     protected const  TYPE_HOUR = 2;
//     protected const  TYPE_DAY = 3;
//
//
//
//     protected $seconds = 0;
//
//     public function __construct () {
//
//     }
//
//     // 运行函数
//     public function run (Callable $func) {
//         $func();
//     }
//
//     // 设置时间长度
//     public function  setTime (int $num, string $type = 'sec') {
//         switch ($type) {
//             case self::TYPE_SEC :
//                 $this->$sec = $nm;
//                 return;
//             case self::TYPE_SEC :
//                 $this->$sec = $nm;
//                 return;
//             case self::TYPE_SEC:
//                 $this->$sec = $nm;
//                 return;
//             case self::TYPE_SEC:
//                 $this->$sec = $nm;
//                 return;
//             default:
//                 error to not match type
//         }
//     }
//
//     // 进程锁: 等待耗时长任务完成, 减少进程堆积
//     public function lock () {
//
//     }
//
//     // 错误处理函数
//     public function errorHandler (Callable $func) {
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
        sleep(5);
    }
}

function test ($a, $b) {
     echo $a .PHP_EOL;
     echo $b .PHP_EOL;
}
// ZEND_DONT_UNLOAD_MODULES=1 USE_ZEND_ALLOC=0 valgrind  --leak-check=full --log-file=./test1.log --track-origins=yes php test.php


// Task::run('test', 'a', 'b');
// (new Task(2))->test();
// $a = 2;
// (new Task($a))::run('test', 'a', 'b');

// 测试再定时那未完成任务
// 定时1 秒, sleep 5 秒会等待5 结束后再执行
$t = new Task();
// $t::failHandler(function () {
//     echo 1111;
// });
$t->setTime(1);
$t->run('test', 'a', 'b');
// var_dump(Task::HOUR);

// $t->setTime(1, TASK::HOUR);
var_dump(Task::$sec);