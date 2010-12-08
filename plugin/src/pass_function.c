#include "myproof.h"

static const char *context = "pass function";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_myproof_function *create_function_struct( const char *name )
{
    t_myproof_function *function = xmalloc( sizeof(*function) );
    strcpy( function->name, name );
    mylist_init( &(function->variables) );
    mylist_init( &(function->basicblocks) );
    mylist_init( &(function->loops) );
    mylist_init( &(function->edges) );
    return function;
}

unsigned int pass_function()
{
    warning(0, "%<%s%>", context);

    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    if ( mylist_find( g_myproof_pass->functions, function_exists, (void*)identifier ) == NULL )
	{
	    mylist_insert( &(g_myproof_pass->functions), create_function_struct( identifier ) );
	}

    return 0;
}
