//
// Created by vj-zhou on 2021/12/3.
//

#ifndef TIMER_TASK_H
#define TIMER_TASK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_timer.h"


//typedef struct _task {
//    struct timeval tv;
//    void (*func)(void*);
//    void *data;
//} Task;

typedef struct _fi {
    zend_fcall_info_cache  fcc;
    zend_fcall_info fci;
} Fi;

extern  zend_class_entry *task_class_entry_p;
enum {SECOND=0, MINUTE, HOUR, DAY};

TIMER_MINIT_FUNCTION(task);

#endif //TIMER_TASK_H

