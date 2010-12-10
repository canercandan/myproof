#include "myproof.h"

static const char *context = "pass edge";

static t_myproof_edge *create_edge_struct( t_myproof_basicblock *src, t_myproof_basicblock *dst )
{
    t_myproof_edge *edge = xmalloc( sizeof(*edge) );
    edge->src = src;
    edge->dst = dst;
    return edge;
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

static t_mylist_res edge_exists( void *data, void *user_data )
{
    t_myproof_edge* edge = data;
    void **args = user_data;
    t_myproof_basicblock *src = args[0];
    t_myproof_basicblock *dst = args[1];
    return ( edge->src == src && edge->dst == dst ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

unsigned int pass_edge()
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
		fprintf( stderr, "myproof: pass_bb: unhandled \'%d\' basicblock\n", (size_t)bb->index );
		return 0;
    	    }

	if (bb->count < 0)
	    {
		error("corrupted profile info: number of iterations for basic block %d thought to be %i", bb->index, (int)bb->count);
		bb->count = 0;
	    }

	edge e;
	edge_iterator ei;

	FOR_EACH_EDGE( e, ei, bb->succs )
	    {
		if ( (e->count < 0 && e->dest == EXIT_BLOCK_PTR) || (e->count > bb->count && e->dest != EXIT_BLOCK_PTR) ) { continue; }
		if ( e->flags & EDGE_FALLTHRU ) { break; }
		if ( !e->src || !e->dest ) { continue; }

		if ( (size_t)e->src->index != index )
		    {
			error("edge src must be equal to bb index");
		    }

		t_myproof_basicblock *dst = mylist_find( function->basicblocks, basicblock_exists, (void*)e->dest->index );

		if ( dst == NULL && e->dest->index != 1 )
		    {
			warning(0, "%<%s%> unable to find edge dest from basicblocs", context);
		    }

		void *args[2] = { basicblock, dst };
		t_myproof_edge *edge = mylist_find( function->edges, edge_exists, (void*)args );

		if ( edge == NULL )
		    {
			edge = create_edge_struct( basicblock, (e->dest->index == 1) ? NULL : dst );
			mylist_insert( &(function->edges), edge );
		    }
	    }
    }

    return 0;
}
