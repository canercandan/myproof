#ifndef __MYPROOF_H__
# define __MYPROOF_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

    // headers used for gcc plugin
#include "gcc-plugin.h"
#include "plugin-version.h"
    // headers used for gcc plugin end

    // header files needed by gcc gimple
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "function.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "cfgloop.h"
#include "intl.h"
    // header files needed by gcc gimple end

    // headers used for pragma
#include "c-pragma.h"
#include "cpplib.h"
    // headers used for pragma end

#include "toplev.h" // warning function

#include "mylist.h" // get a linked-list

    /*
    ** defines
    */

    #define MYPROOF_PRAGMA_SPACE "myproof"

    /*
    ** type definitions
    */

    typedef struct s_myproof_variable
    {
	char name[128];
	size_t size;
	size_t IR;
	size_t IW;
	size_t FR;
	size_t FW;
    } t_variable;

    typedef struct
    {
	t_mylist *variables;
	size_t num_all_ops;
    } t_myproof;

    typedef enum
	{
	    MYPROOF_PRAGMA_NONE = 0,
	    MYPROOF_PRAGMA_TEST
	} t_myproof_pragma_kind;

    typedef enum
	{
	    MYPROOF_PRAGMA_CLAUSE_NONE = 0
	} t_myproof_pragma_clause;

    typedef void (*t_myproof_pragma_handler)( cpp_reader* );

    typedef struct
    {
	const char *name;
	unsigned int id;
	t_myproof_pragma_handler handler;
    } t_myproof_pragma_def;

    /*
    ** structures
    */

    /*
    ** functions
    */

    int plugin_init( struct plugin_name_args*, struct plugin_gcc_version* );
    void plugin_ends( void *gcc_data, void *user_data );
    void plugin_summary( void *gcc_data, void *user_data );
    void plugin_pragma( void *gcc_data, void *user_data );

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYPROOF_H__ */
