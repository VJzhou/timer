//
// Created by vj-zhou on 2021/12/3.
//

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_timer.h"
#include "task.h"

#define WAIT_START 1
#define DEFAULT_SEC 0

zend_class_entry *task_class_entry_p;
static sigset_t alarm_set;
Fi fi;

//===========方法申明
static PHP_METHOD(Task, run);
void init_sigaction ();
void init_timer();


void init_sigaction (void (*func)()) {
    struct sigaction act;
    act.sa_handler = func;

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGINT, &act, NULL);

    sigemptyset(&alarm_set);
    sigaddset(&alarm_set, SIGALRM);
}

void alarm_handler (int signo) {

    if (signo == SIGINT) {
        php_printf("exited");
        exit(0);
    }

    if (zend_call_function(&fi.fci, &fi.fcc) != SUCCESS) {
        // todo log 可以一个函数, 这个函数处理失败情况
        php_printf("call func error...\n");
        exit(1);
    }
}

void init_timer(struct timeval interval) {
    struct itimerval itv;
    itv.it_interval = interval;

    itv.it_value.tv_sec = WAIT_START; // 1秒后启动定时器
    itv.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &itv, NULL);
}

PHP_METHOD(Task, __construct)
{
    zend_long sec = DEFAULT_SEC;
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(sec)
    ZEND_PARSE_PARAMETERS_END();

    zend_update_static_property_long(Z_OBJCE_P(ZEND_THIS), "sec", sizeof("sec") -1, sec);
}

PHP_METHOD(Task, run)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    zval result, tmp;

    ZEND_PARSE_PARAMETERS_START(1,-1)
    Z_PARAM_FUNC(fci, fcc)
    Z_PARAM_VARIADIC("*", fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE); // 参数解析失败返回false
    fci.retval = &result;

//    *return_value = result;

    fi.fci = fci;
    fi.fcc = fcc;

//    zval *zv = zend_read_property(task_class_entry_p, ZEND_THIS, "sec", sizeof("sec")-1, 0, NULL);
    zval *zv = zend_read_static_property(task_class_entry_p, "sec", sizeof("sec")-1, 0);
    zend_long sec = zval_get_long(zv);

    // read property of sec's  value
    if (UNEXPECTED(sec == 0)) {
        php_error_docref(NULL, E_ERROR, "sec must be grand than 0");
        RETURN_NULL();
    }


    struct timeval tv, interval;
    php_printf("sec %d", sec);
    interval.tv_sec = sec;
    interval.tv_usec = 0;

    tv.tv_sec = 1; //  阻塞时间
    tv.tv_usec = 0;

    init_sigaction(alarm_handler);
    init_timer(interval);

    while (1) {
        select(0, NULL, NULL, NULL, &tv);
    }
}


//===========方法参数
ZEND_BEGIN_ARG_INFO(arginfo_task_construct, 0)
ZEND_ARG_INFO(0, sec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_task_run, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()


static const zend_function_entry task_functions[] = {
        PHP_ME(Task, __construct, arginfo_task_construct, ZEND_ACC_PUBLIC)
        ZEND_ME(Task, run, arginfo_task_run, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        ZEND_FE_END
};

TIMER_MINIT_FUNCTION(task){
    zend_class_entry task_class_entry;
    INIT_CLASS_ENTRY(task_class_entry, "Task", task_functions);
    task_class_entry_p = zend_register_internal_class(&task_class_entry TSRMLS_CC);
    zend_declare_property_long(task_class_entry_p, "sec", sizeof("sec")-1, DEFAULT_SEC, ZEND_ACC_PROTECTED | ZEND_ACC_STATIC);
    return SUCCESS;
}






