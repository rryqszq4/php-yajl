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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_smart_str.h"
#include "php_yajl.h"
#include "yajl/api/yajl_version.h"
#include "yajl/api/yajl_gen.h"

/* If you declare any globals in php_yajl.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(yajl)
*/

/* True global resources - no need for thread safety here */
static int le_yajl;

static void php_yajl_generate(yajl_gen gen, zval *val);

PHP_FUNCTION(yajl_version);
PHP_FUNCTION(yajl_generate);
PHP_FUNCTION(yajl_parse);

ZEND_BEGIN_ARG_INFO_EX(arginfo_yajl_generate, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ yajl_functions[]
 *
 * Every user visible function must have an entry in yajl_functions[].
 */
const zend_function_entry yajl_functions[] = {
	PHP_FE(confirm_yajl_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(yajl_version, NULL)
	PHP_FE(yajl_generate, arginfo_yajl_generate)
	PHP_FE_END	/* Must be the last line in yajl_functions[] */
};
/* }}} */

/* {{{ yajl_module_entry
 */
zend_module_entry yajl_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"yajl",
	yajl_functions,
	PHP_MINIT(yajl),
	PHP_MSHUTDOWN(yajl),
	PHP_RINIT(yajl),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(yajl),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(yajl),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_YAJL
ZEND_GET_MODULE(yajl)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("yajl.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_yajl_globals, yajl_globals)
    STD_PHP_INI_ENTRY("yajl.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_yajl_globals, yajl_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_yajl_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_yajl_init_globals(zend_yajl_globals *yajl_globals)
{
	yajl_globals->global_value = 0;
	yajl_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(yajl)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(yajl)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(yajl)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(yajl)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(yajl)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yajl support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_yajl_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_yajl_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "yajl", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(yajl_version)
{
	php_printf("yajl version: %d\n", yajl_version());
}

static void php_yajl_generate_array(yajl_gen gen, zval *val)
{
	yajl_gen_array_open(gen);

	HashTable *hash_table;
	int i;

	hash_table = HASH_OF(val);

	if (hash_table && hash_table->nApplyCount > 1) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "recursion detected");
		yajl_gen_null(gen);
		return;
	}

	i = hash_table ? zend_hash_num_elements(hash_table) : 0;

	if (i > 0)
	{
		HashPosition pos;
		char *key;
		ulong index;
		uint key_len;
		zval **data;
		HashTable *tmp_ht;

		zend_hash_internal_pointer_reset_ex(hash_table, &pos);
		for (;; zend_hash_move_forward_ex(hash_table, &pos))
		{
			i = zend_hash_get_current_key_ex(hash_table, &key, &key_len, &index, 0, &pos);
			if (i == HASH_KEY_NON_EXISTANT)
				break;

			if (zend_hash_get_current_data_ex(hash_table, (void **)&data, &pos) == SUCCESS)
			{
				tmp_ht = HASH_OF(*data);
				if (tmp_ht){
					tmp_ht->nApplyCount++;
				}

				if (i == HASH_KEY_IS_STRING){
					yajl_gen_string(gen, key, key_len-1);
				}else {
					yajl_gen_integer(gen, i);
				}

				php_yajl_generate(gen, *data);
			}
		}
	}

	yajl_gen_array_close(gen);

}

static void php_yajl_generate(yajl_gen gen, zval *val)
{
	switch(Z_TYPE_P(val))
	{
		case IS_NULL:
			yajl_gen_null(gen);
			break;

		case IS_BOOL:
			if (Z_BVAL_P(val)){
				yajl_gen_bool(gen, 1);
			}else {
				yajl_gen_bool(gen, 0);
			}
			break;

		case IS_LONG:
			yajl_gen_integer(gen, Z_LVAL_P(val));
			break;

		case IS_DOUBLE:
			yajl_gen_double(gen, Z_DVAL_P(val));
			break;

		case IS_STRING:
			yajl_gen_string(gen, Z_STRVAL_P(val), Z_STRLEN_P(val));
			break;

		case IS_ARRAY:
			php_yajl_generate_array(gen, val);
			break;

		default:
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "type is unsupported, generate as null");
			yajl_gen_null(gen);
			break;
	}

	return ;
}

PHP_FUNCTION(yajl_generate)
{
	zval *param;
	smart_str buf = {0};
	yajl_gen gen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &param) == FAILURE)
	{
		return ;
	}

	gen = yajl_gen_alloc(NULL);
	yajl_gen_config(gen, yajl_gen_beautify, 1);

	/*unsigned char *key = "key";
	unsigned char *value = "value";
	int key_len = strlen(key);
	int value_len = strlen(value);

	yajl_gen_array_open(gen);

	yajl_gen_string(gen, (const unsigned char*)key, key_len);
	yajl_gen_string(gen, (const unsigned char*)value, value_len);

	yajl_gen_array_close(gen);
	*/
	
	php_yajl_generate(gen, param);

	yajl_gen_get_buf(gen, &buf.c, &buf.len);

	ZVAL_STRINGL(return_value, buf.c, buf.len, 1);

	yajl_gen_free(gen);

	smart_str_free(&buf);

}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
