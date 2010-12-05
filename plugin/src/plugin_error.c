#include "myproof.h"

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_mylist_res for_each_instrumente_function( void *data, void *user_data )
{
    t_myproof *myproof = user_data;
    t_myproof_instrumente_function* instrumente_function = data;
    if ( mylist_all_data( myproof->functions, function_exists, instrumente_function->name ) == MYLIST_R_CONTINUE )
	{
	    fprintf( stderr, "%s does not exist\n", instrumente_function->name );
	}
    return MYLIST_R_CONTINUE;
}

void plugin_error( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    t_myproof *myproof = user_data;
    mylist_all_data( myproof->instrumente_functions, for_each_instrumente_function, myproof );
}
