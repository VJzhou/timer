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
static PHP_METHOD(Task, setTime);
static PHP_METHOD(Task, flock);
void init_sigaction ();
void init_timer();

//===========方法实现
void init_sigaction (void (*func)()) {
    struct sigaction act;
    act.sa_handler = func;

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGINT, &act, NULL);
}

// 捕获CTRL C 信号, 处理
static zend_always_inline void timer_exit (int signo) {
    php_printf("exited");
    exit(0);
}

void alarm_handler (int signo) {
    switch (signo) {
        case SIGINT :
            timer_exit(signo);
        case SIGALRM:
            if (zend_call_function(&fi.fci, &fi.fcc) == FAILURE) {
                zval_ptr_dtor(&fi);
                zval_ptr_dtor(&fi);
                php_printf("execute failed");
                exit(1);
            }
            break;
        default:
            php_printf("default....\n");
    }
}

void init_timer(struct timeval interval) {
    struct itimerval itv;
    itv.it_interval = interval;

    itv.it_value.tv_sec = 0;  // 0.1秒后启动定时器
    itv.it_value.tv_usec = 100;

    setitimer(ITIMER_REAL, &itv, NULL);
}

// php function

static
PHP_METHOD(Task, __construct)
{
    zend_update_static_property_long(Z_OBJCE_P(ZEND_THIS), "sec", sizeof("sec") -1, DEFAULT_SEC);
}

static
PHP_METHOD(Task, setTime)
{
    zend_long time, type;
    ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_LONG(time)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(type)
    ZEND_PARSE_PARAMETERS_END();

    if (ZEND_NUM_ARGS() < 2) {
        type = SECOND;
    }

    switch (type)
    {
        case SECOND:
            zend_update_static_property_long(Z_OBJCE_P(ZEND_THIS), "sec", sizeof("sec") -1, time);break;
        case MINUTE:
            time *= 60;
            zend_update_static_property_long(Z_OBJCE_P(ZEND_THIS), "sec", sizeof("sec") -1, time);break;
        case HOUR:
            time *= 60*60;
            zend_update_static_property_long(Z_OBJCE_P(ZEND_THIS), "sec", sizeof("sec") -1, time);break;
        case DAY:
            time *= 60*60*24;
            zend_update_static_property_long(Z_OBJCE_P(ZEND_THIS), "sec", sizeof("sec") -1, time);break;
        default:
            php_error_docref(NULL, E_ERROR, "does not match type");
            return ;
    }
}

static
PHP_METHOD(Task, run)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    zval result;

    ZEND_PARSE_PARAMETERS_START(1,-1)
    Z_PARAM_FUNC(fci, fcc)
    Z_PARAM_VARIADIC("*", fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE); // 参数解析失败返回false
    fci.retval = &result;

    fi.fci = fci;
    fi.fcc = fcc;

    zval *zv = zend_read_static_property(task_class_entry_p, "sec", sizeof("sec")-1, 0);
    zend_long sec = zval_get_long(zv);

    if (UNEXPECTED(sec == 0)) {
        php_error_docref(NULL, E_ERROR, "must be set time of timer interval in setTime()");
        RETURN_NULL();
    }

    struct timeval tv, interval;
    interval.tv_sec = sec;
    interval.tv_usec = 0;

    tv.tv_sec = 0; //  阻塞时间
    tv.tv_usec = 0;

    init_sigaction(alarm_handler);
    init_timer(interval);

    while (1) {
        select(0, NULL, NULL, NULL, &tv);
    }
}

static
PHP_METHOD(Task, failHandler)
{
    zend_fcall_info fci = empty_fcall_info;
    zend_fcall_info_cache fcc = empty_fcall_info_cache;
    zval result, tmp;

    ZEND_PARSE_PARAMETERS_START(1,-1)
    Z_PARAM_FUNC(fci, fcc)
    Z_PARAM_VARIADIC("*", fci.params, fci.param_count)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE); // 参数解析失败返回false
    fci.retval = &result;

    fail_fi.fci = fci;
    fail_fi.fcc = fcc;
}


//===========方法参数
ZEND_BEGIN_ARG_INFO(arginfo_task_construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_task_run, 0, 0, 1)
    ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_task_failHandler, 0, 0, 1)
ZEND_ARG_CALLABLE_INFO(0, func, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_task_setTime, 0, 2, IS_VOID, 0) // name , ref, r_num, type, allow_null
    ZEND_ARG_TYPE_INFO(0, time, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry task_functions[] = {
        PHP_ME(Task, __construct, arginfo_task_construct, ZEND_ACC_PUBLIC)
        ZEND_ME(Task, run, arginfo_task_run, ZEND_ACC_PUBLIC)
        ZEND_ME(Task, failHandler, arginfo_task_failHandler, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
        ZEND_ME(Task, setTime, arginfo_task_setTime, ZEND_ACC_PUBLIC)
        ZEND_FE_END
};

TIMER_MINIT_FUNCTION(task){
    zend_class_entry task_class_entry;
    INIT_CLASS_ENTRY(task_class_entry, "Task", task_functions);
    task_class_entry_p = zend_register_internal_class(&task_class_entry TSRMLS_CC);
    zend_declare_property_long(task_class_entry_p, "sec", sizeof("sec")-1, DEFAULT_SEC, ZEND_ACC_PROTECTED | ZEND_ACC_STATIC);

    zend_declare_class_constant_long(task_class_entry_p, "SECOND", strlen("SECOND"), SECOND);
    zend_declare_class_constant_long(task_class_entry_p, "HOUR", strlen("HOUR"), HOUR);
    zend_declare_class_constant_long(task_class_entry_p, "DAY", strlen("DAY"), DAY);

    return SUCCESS;
}






