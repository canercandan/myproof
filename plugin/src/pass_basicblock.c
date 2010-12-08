#include "myproof.h"

static const char *context = "pass basicblock";

static t_myproof_basicblock *create_basicblock_struct( const size_t index )
{
    t_myproof_basicblock *basicblock = xmalloc( sizeof(*basicblock) );
    basicblock->index = index;
    basicblock->nload = 0;
    basicblock->nstore = 0;
    basicblock->loop = NULL;
    return basicblock;
}

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_mylist_res basicblock_exists( void *data, void *user_data )
{
    t_myproof_basicblock* basicblock = data;
    return ( basicblock->index == (size_t)user_data ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
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
    	size_t index = bb->index;

    	t_myproof_basicblock *basicblock = mylist_find( function->basicblocks, basicblock_exists, (void*)index );

    	if ( basicblock == NULL )
    	    {
    		basicblock = create_basicblock_struct( index );
    		mylist_insert( &(function->basicblocks), basicblock );
    	    }
    }

    return 0;
}
