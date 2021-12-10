/* timer extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_timer.h"
#include "task.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

PHP_MINIT_FUNCTION(timer)
{
    TIMER_STARTUP(task);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(timer)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(timer)
{
    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(timer)
{
#if defined(ZTS) && defined(COMPILE_DL_TIMER)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(timer)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "timer support", "enabled");
	php_info_print_table_end();
}
/* }}} */

zend_function_entry timer_functions[] = {
        NULL, NULL, NULL
};

/* {{{ timer_module_entry */
zend_module_entry timer_module_entry = {
	STANDARD_MODULE_HEADER,
	"timer",					/* Extension name */
    timer_functions,					/* zend_function_entry */
    PHP_MINIT(timer),							/* PHP_MINIT - Module initialization */
	PHP_MSHUTDOWN(timer),							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(timer),			/* PHP_RINIT - Request initialization */
    PHP_RSHUTDOWN(timer),							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(timer),			/* PHP_MINFO - Module info */
	PHP_TIMER_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TIMER
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(timer)
#endif
