<?php
class Task {

    const SECOND = 0;
    const MINUTE = 0;
    const HOUR = 0;
    const DAY = 0;

    protected static $sec;
    protected static $is_lock;
    protected static $filepath;

    public function __construct ()
    {
        self::$sec = 0;
    }

    public function setTime ($time, $type = self::SECOND) // type类型为Task常量
    {
        switch ($type) {
            case self::SECOND:
                self::$sec = $time;
                break;
            case self::MINUTE:
                self::$sec = $time * 60;
                break;
            case self::HOUR:
                self::$sec = $time * 60 * 60;
                break;
            case self::DAY:
                self::$sec = $time * 60 * 60 * 24;
                break;
            default :
                throw New \Error("does not match type");
        }
    }

    public function run (Callback $func, ...$argv)
    {
        while(1) {
            if (self::$is_lock) {
                continue;
            }
            $func($argv);
            self::$is_lock = 0;
            sleep(self::$sec);
        }
    }

    public function flock($filepath)
    {
        if (!file_exists($filepath)) {
            // todo 创建文件
        }
    }
}