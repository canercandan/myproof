#include "myproof.h"

static const char *context = "pass basicblock";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_mylist_res basicblock_exists( void *data, void *user_data )
{
    t_myproof_basicblock* basicblock = data;
    return ( basicblock->index == (unsigned int)user_data ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_myproof_basicblock *create_basicblock_struct( const unsigned int index )
{
    t_myproof_basicblock *basicblock = xmalloc( sizeof(*basicblock) );
    basicblock->index = index;
    mylist_init( &(basicblock->variables) );
    return basicblock;
}

unsigned int pass_basicblock()
{
    warning(0, "%<%s%>", context);

    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    t_myproof_function *function = mylist_find( g_myproof_pass->functions, function_exists, (void*)identifier );

    if ( function == NULL )
	{
	    fprintf( stderr, "myproof: pass_basicblock: unhandled \'%s\' function\n", identifier );
	    return 0;
	}

    basic_block bb;

    FOR_EACH_BB( bb )
    {
	unsigned int index = bb->index;

	if ( mylist_find( function->basicblocks, basicblock_exists, (void*)index ) == NULL )
	    {
		mylist_push( &(function->basicblocks), create_basicblock_struct( index ) );
	    }
    }

    return 0;
}
