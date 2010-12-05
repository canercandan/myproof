#include "myproof.h"

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_instrumente_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_myproof_function *create_function_struct( const char *name )
{
    t_myproof_function *function = xmalloc( sizeof(*function) );
    strcpy( function->name, name );
    function->visited = 0;
    mylist_init( &(function->variables) );
    return function;
}

unsigned int pass_function()
{
    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    if ( mylist_all_data( g_myproof_pass->functions, function_exists, (void*)identifier ) == MYLIST_R_CONTINUE )
	{
	    mylist_push( &(g_myproof_pass->functions), create_function_struct( identifier ) );
	}

    return 0;
}
