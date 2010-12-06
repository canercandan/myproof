#include "myproof.h"

static const char *context = "pass instrumente";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_instrumente_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

tree create_function_call( const char *fname, const char *arg )
{
    tree ftype = build_function_type_list( void_type_node /*return*/, ptr_type_node /*first arg*/, NULL_TREE );
    tree fndecl = build_fn_decl( fname, ftype );
    tree fs = build_string( strlen(arg) + 1, arg );

    DECL_ASSEMBLER_NAME( fndecl );
    TREE_TYPE( fs ) = build_array_type( char_type_node, build_index_type( build_int_cst( NULL_TREE, strlen( arg ) ) ) );
    TREE_CONSTANT( fs ) = 1;
    TREE_READONLY( fs ) = 1;
    TREE_STATIC( fs ) = 1;
    fs = build1( ADDR_EXPR, build_pointer_type( char_type_node ), fs );

    tree fargs = tree_cons( NULL_TREE, fs, NULL_TREE );

    return build_function_call_expr( UNKNOWN_LOCATION, fndecl, fargs );
}

// http://svn.gna.org/svn/gsc/branches/hello-world/5-debug/hello1.c

unsigned int pass_instrumente()
{
    warning(0, "%<%s%>", context);

    tree t = cfun->decl;
    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(t));

    if ( mylist_find( g_myproof_pass->instrumente_functions, function_exists, (void*)identifier ) == NULL )
	{
	    fprintf( stderr, "%s is not instrumentable\n", identifier );
	    return 0;
	}

    //debug_tree( t );

    tree call = create_function_call( "test", "toto" );

    gimple stmt = gimple_build_call_from_tree( call );

    debug_gimple_stmt( stmt );

    basic_block bb = cfun->cfg->x_entry_block_ptr->next_bb;
    gimple_stmt_iterator gsi = gsi_start_bb( bb );
    gsi_insert_before( &gsi, stmt, GSI_SAME_STMT );

    //debug_tree( t );

    //gimple_build_call_from_tree( test_fndecl );

    //dump_function( TDI_original, test_fndecl );
    //gimplify_function_tree( test_fndecl );
    //dump_function( TDI_original, test_fndecl );

    /* debug_tree(t); */

    fprintf( stderr, "* MYPROOF on %s()\n", identifier );

    // gsi_insert_on_edge_immediate

    /* basic_block bb; */

    /* FOR_EACH_BB( bb ) */
    /* { */
    /* 	edge e; */
    /* 	edge_iterator ei; */

    /* 	FOR_EACH_EDGE (e, ei, bb->succs) */
    /* 	    { */
    /* 		if (e->flags & EDGE_FALLTHRU) */
    /* 		    { */
    /* 			break; */
    /* 		    } */
    /* 	    } */
    /* } */

    return 0;
}
