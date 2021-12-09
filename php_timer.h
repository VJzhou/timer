/* timer extension for PHP */

#ifndef PHP_TIMER_H
# define PHP_TIMER_H

extern zend_module_entry timer_module_entry;
# define phpext_timer_ptr &timer_module_entry

# define PHP_TIMER_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_TIMER)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#define TIMER_MINIT_FUNCTION(module)   	    ZEND_MINIT_FUNCTION(timer_##module)
#define TIMER_STARTUP(module)	 		ZEND_MODULE_STARTUP_N(timer_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define TIMER_SHUTDOWN_FUNCTION(module)  	ZEND_MSHUTDOWN_FUNCTION(timer_##module)
#define TIMER_SHUTDOWN(module)	 	    ZEND_MODULE_SHUTDOWN_N(timer_##module)(INIT_FUNC_ARGS_PASSTHRU)


PHP_MINIT_FUNCTION(timer);
#endif	/* PHP_TIMER_H */
