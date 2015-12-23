/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: rryqszq4                                                     |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_YAJL_H
#define PHP_YAJL_H

extern zend_module_entry yajl_module_entry;
#define phpext_yajl_ptr &yajl_module_entry

#ifdef PHP_WIN32
#	define PHP_YAJL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_YAJL_API __attribute__ ((visibility("default")))
#else
#	define PHP_YAJL_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(yajl);
PHP_MSHUTDOWN_FUNCTION(yajl);
PHP_RINIT_FUNCTION(yajl);
PHP_RSHUTDOWN_FUNCTION(yajl);
PHP_MINFO_FUNCTION(yajl);

PHP_FUNCTION(confirm_yajl_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(yajl)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(yajl)
*/

/* In every utility function you add that needs to use variables 
   in php_yajl_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as YAJL_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define YAJL_G(v) TSRMG(yajl_globals_id, zend_yajl_globals *, v)
#else
#define YAJL_G(v) (yajl_globals.v)
#endif

#endif	/* PHP_YAJL_H */

static int json_determine_array_type(zval **val TSRMLS_DC);
static void php_yajl_generate_array(yajl_gen gen, zval *val TSRMLS_DC);
static void php_yajl_generate(yajl_gen gen, zval *val TSRMLS_DC);

static int object_add_keyval(context_t *ctx, zval *obj, zval *key, zval *value);
static int array_add_value (context_t *ctx,
                            zval *array, zval *value);
static int context_push(context_t *ctx, zval *v, int type);
static zval* context_pop(context_t *ctx);
static int context_add_value (context_t *ctx, zval *v);
static int handle_string (void *ctx,
                          const unsigned char *string, size_t string_length);
static int handle_number (void *ctx, const char *string, size_t string_length);
static int handle_integer(void *ctx, long long int value);
static int handle_double(void *ctx, double value);
static int handle_start_map (void *ctx);
static int handle_end_map (void *ctx);
static int handle_start_array (void *ctx);
static int handle_end_array (void *ctx);
static int handle_boolean (void *ctx, int boolean_value);
static int handle_null (void *ctx);
zval* yajl_zval_parse (const char *input,
                          char *error_buffer, size_t error_buffer_size);
void yajl_zval_free (zval *v);

PHP_FUNCTION(yajl_version);
PHP_FUNCTION(yajl_generate);
PHP_FUNCTION(yajl_parse);


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
