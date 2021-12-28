# Timer

PHP 定时任务扩展,用于php-cli

# 编译

```
    ./travis/complie.sh
```

# 使用,详细可看Task.stub.php


* 设置间隔时间  

    $t->setTime(1);


* run 
    
    $t->run();


# Demo

```
function test($a, $b)
{
    echo $a .PHP_EOL;
    echo $b .PHP_EOL;
}

$t = new Task;
$t->run('test', 'a参数', 'b参数');
```


