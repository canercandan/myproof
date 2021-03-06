// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Authors:
 * Caner Candan <caner@candan.fr>, http://caner.candan.fr
 * Aurèle Mahéo <aurele.maheo@gmail.com>
 */

#include "myproof.h"

static const char *context = "pass generic";

static int get_type_size( tree t )
{
    int ret = 0;

    if ( TYPE_SIZE_UNIT(t) != NULL )
	{
	    ret = TREE_INT_CST_LOW( TYPE_SIZE_UNIT(t) );
	}

    return ret;
}

static int get_size( tree t )
{
    enum tree_code tc;

    tc = TREE_CODE( t );

    int size = -1;

    switch ( tc )
	{
	case BOOLEAN_TYPE:
	case INTEGER_TYPE:
	case REAL_TYPE:
	    size = get_type_size( t ); break;

	default:
	    fprintf( stderr, "myproof: get_size(): %s is not handled\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}

    return size;
}

static void read_type( tree t )
{
    enum tree_code tc;

    tc = TREE_CODE( t );

    switch ( tc )
	{
	case INTEGER_TYPE:
	case BOOLEAN_TYPE:
	case REAL_TYPE:
	    /* scalar */
	    get_size( t );
	    /* printf( "get_size: %d\n", get_size( t ) ); */

	    break;

	case POINTER_TYPE:
	    break;

	default:
	    fprintf( stderr, "myproof: read_type(): %s is not handled\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}
}

static void read_data( tree t )
{
    if ( DECL_NAME(t) )
	{
	    /* printf ( "       data \'%s\'\n", IDENTIFIER_POINTER(DECL_NAME(t)) ); */
	}
    else
	{
	    /* printf ( "       data \'%c_%u\'\n", (TREE_CODE(t)==CONST_DECL)?'C':'D', DECL_UID(t) ); */
	}

    read_type ( TREE_TYPE(t) );
}

static void read_operand( tree t )
{
    enum tree_code tc;

    tc = TREE_CODE( t );

    /* tree tree_new = TREE_TYPE( t ); */
    /* enum tree_code tc_new = TREE_CODE ( t ); */

    /* if ( tc_new == REAL_TYPE ) */
    /* 	{ */
    /* 	    printf("this is a real variable\n"); */
    /* 	} */
    /* else if ( tc_new == INTEGER_TYPE ) */
    /* 	{ */
    /* 	    printf("this is an integer variable\n"); */
    /* 	} */

    switch ( tc )
	{
	case VAR_DECL:
	case PARM_DECL:
	case CONST_DECL:
	    read_data( t );
	    break;

	case ARRAY_REF:
	    read_operand( TREE_OPERAND(t,0) ); /* array base */
	    read_operand( TREE_OPERAND(t,1) ); /* array index */
	    break;

	case ADDR_EXPR:
	case INDIRECT_REF:
	    /* pointer & dereferencing */
	    break;

	case INTEGER_CST:
	case REAL_CST:
	case STRING_CST:
	    /* integer/real/string constant */
	    break;

	case SSA_NAME:
	    read_data( SSA_NAME_VAR(t) );
	    break;

	case COND_EXPR:
	case TARGET_MEM_REF:
	    break;

	default:
	    fprintf( stderr, "myproof: read_operand(): unhandled \'%s\'\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}
}

static void read_stmt( gimple g )
{
    unsigned int i;
    enum gimple_code gc;

    gc = gimple_code( g );

    /* debug_tree(type1); */

    //debug_tree(type1);

    /* switch ( gc ) */
    /* 	{ */
    /* 	case GIMPLE_ASSIGN: */
    /* 	case GIMPLE_CALL: */
    /* 	case GIMPLE_COND: */
    /* 	case GIMPLE_RETURN: */
    /* 	    num_all_ops += gimple_num_ops(g); */
    /* 	} */

    switch ( gc )
	{
	case GIMPLE_ASSIGN:
	    read_operand( gimple_op(g,1) ); /* op1 */
	    /* num_all_ops_write++; */

	    tree type1 = TREE_TYPE( gimple_op( g, 1 ) );
	    int tc = TREE_CODE( type1 );

	    if ( tc == REAL_TYPE )
		{
		    //debug_tree(type1);
		    /* printf("this is a real variable\n"); */
		}

	    /* enum tree_code tr = gimple_assign_rhs_code(g); */
	    /* printf("Doing %s\n", tree_code_name[tr]); */

	    if ( gimple_num_ops(g) > 2 ) /* op2 */
		{
		    read_operand( gimple_op(g,2) );
		}

	    read_operand( gimple_op(g,0) ); /* op def */
	    break;

	case GIMPLE_CALL:
	    for ( i = 0; i < gimple_call_num_args(g); ++i )
		{
		    read_operand( gimple_call_arg(g,i) );
		}

	    if ( gimple_call_lhs(g) != NULL_TREE )
		{
		    read_operand( gimple_call_lhs(g) );
		}
	    break;

	case GIMPLE_COND:
	    read_operand( gimple_cond_lhs(g) ); /* op1 */
	    read_operand( gimple_cond_rhs(g) ); /* op2 */
	    break;

	case GIMPLE_RETURN:
	    if ( gimple_return_retval(g) != NULL_TREE )
		{
		    read_operand( gimple_return_retval(g) );
		}
	    break;

	case GIMPLE_DEBUG: break;

	default:
	    fprintf( stderr, "myproof: read_stmt(): unhandled \'%s\'\n", gimple_code_name[gc] );
	    gcc_unreachable ( );
	}
}

static void read_loop( struct loop *l )
{
    struct loop *inner;

    /* fprintf( stderr, " # loop %d", l->num ); */

    if ( double_int_zero_p(l->nb_iterations_estimate) ) /* double_int type defined in double-int.h */
	{
	    /* fprintf( stderr, " has unknown number of iterations" ); */
	}
    else
	{
	    /* fprintf( stderr, " has %llu iterations", l->nb_iterations_estimate.low ); */
	}

    /* fprintf( stderr, "\n" ); */

    for ( inner = l->inner; inner != NULL; inner = inner->next )
	{
	    read_loop( inner );
	}
}

unsigned int pass_generic()
{
    warning(0, "%<%s%>", context);

    basic_block bb;
    gimple_stmt_iterator gsi;

    /* fprintf( stderr, "* MYPROOF on %s()\n", IDENTIFIER_POINTER(DECL_NAME(cfun->decl)) ); */

    FOR_EACH_BB( bb )
    {
	/* fprintf( stderr, "  ** BB %d\n", bb->index ); */

	for ( gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi) )
	    {
		/* print_gimple_stmt ( stdout, gsi_stmt(gsi), 0, 0 ); */
		read_stmt( gsi_stmt(gsi) );
	    }
    }

    /* recursively read loops */
    if ( cfun->x_current_loops != NULL )
	{
	    read_loop( cfun->x_current_loops->tree_root );
	}

    return 0;
}
