#include "myproof.h"

static const char *context = "pass bb";

static void read_operand( tree t, t_myproof_basicblock *basicblock, bool store )
{
    enum tree_code tc;

    tc = TREE_CODE( t );

    switch ( tc )
	{
	case VAR_DECL:
	case PARM_DECL:
	case CONST_DECL:
	    break;

	case ARRAY_REF:
	    read_operand( TREE_OPERAND(t,0), basicblock, false ); /* array base */
	    read_operand( TREE_OPERAND(t,1), basicblock, false ); /* array index */
	    break;

	case ADDR_EXPR:
	case INDIRECT_REF:
	    /* pointer & dereferencing */
	    if ( store == true ) { basicblock->nstore++; }
	    else { basicblock->nload++; }
	    break;

	case INTEGER_CST:
	case REAL_CST:
	case STRING_CST:
	    /* integer/real/string constant */
	    break;

	case SSA_NAME:
	    break;

	case COND_EXPR:
	case TARGET_MEM_REF:
	    break;

	default:
	    fprintf( stderr, "myproof: read_operand(): unhandled \'%s\'\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}
}

static void read_stmt( gimple g, t_myproof_basicblock *basicblock )
{
    unsigned int i;
    enum gimple_code gc;

    gc = gimple_code( g );

    switch ( gc )
	{
	case GIMPLE_ASSIGN:
	    {
		read_operand( gimple_op(g,1), basicblock, false ); /* op1 */

		if ( gimple_num_ops(g) > 2 ) /* op2 */
		    {
			read_operand( gimple_op(g,2), basicblock, false );
		    }

		read_operand( gimple_op(g,0), basicblock, true ); /* op def */
	    }
	    break;

	case GIMPLE_CALL:
	    for ( i = 0; i < gimple_call_num_args(g); ++i )
		{
		    read_operand( gimple_call_arg(g,i), basicblock, false );
		}

	    if ( gimple_call_lhs(g) != NULL_TREE )
		{
		    read_operand( gimple_call_lhs(g), basicblock, false );
		}
	    break;

	case GIMPLE_COND:
	    read_operand( gimple_cond_lhs(g), basicblock, false ); /* op1 */
	    read_operand( gimple_cond_rhs(g), basicblock, false ); /* op2 */
	    break;

	case GIMPLE_RETURN:
	    if ( gimple_return_retval(g) != NULL_TREE )
		{
		    read_operand( gimple_return_retval(g), basicblock, false );
		}
	    break;

	case GIMPLE_DEBUG: break;

	default:
	    fprintf( stderr, "myproof: read_stmt(): unhandled \'%s\'\n", gimple_code_name[gc] );
	    gcc_unreachable ( );
	}
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

unsigned int pass_bb()
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
    gimple_stmt_iterator gsi;

    FOR_EACH_BB( bb )
    {
    	size_t index = bb->index;

    	t_myproof_basicblock *basicblock = mylist_find( function->basicblocks, basicblock_exists, (void*)index );

    	if ( basicblock == NULL )
    	    {
		fprintf( stderr, "myproof: pass_bb: unhandled \'%d\' basicblock\n", (size_t)bb->index );
		return 0;
    	    }

    	for ( gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi) )
    	    {
    		read_stmt( gsi_stmt(gsi), basicblock );
    	    }
    }

    return 0;
}
