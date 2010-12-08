#include "myproof.h"

static const char *context = "pass variable";

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

	case POINTER_TYPE:
	    break;

	default:
	    fprintf( stderr, "myproof: get_size(): %s is not handled\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}

    return size;
}

static void read_type( tree t, t_myproof_function *function )
{
    (void)function;
    enum tree_code tc;

    tc = TREE_CODE( t );

    switch ( tc )
	{
	case INTEGER_TYPE:
	case BOOLEAN_TYPE:
	case REAL_TYPE:
	    /* scalar */
	    break;

	case POINTER_TYPE:
	    break;

	default:
	    fprintf( stderr, "myproof: read_type(): %s is not handled\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}
}

static t_mylist_res variable_exists( void *data, void *user_data )
{
    t_myproof_variable* variable = data;
    return ( !strcmp(variable->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_myproof_variable *create_variable_struct( const char *name, tree t )
{
    t_myproof_variable *variable = xmalloc( sizeof(*variable) );
    strcpy( variable->name, name );
    variable->size = get_size( TREE_TYPE(t) );
    variable->visited = 0;
    variable->modified = 0;
    return variable;
}

static t_myproof_variable *read_data( tree t, t_myproof_function *function )
{
    char identifier[MYPROOF_NAME_SIZE];

    if ( DECL_NAME(t) )
	{
	    strcpy( identifier, IDENTIFIER_POINTER(DECL_NAME(t)) );
	}
    else
	{
	    sprintf(identifier, "%c_%u", ( TREE_CODE(t) == CONST_DECL ) ? 'C' : 'D', DECL_UID(t) );
	}

    t_myproof_variable *variable = mylist_find( function->variables, variable_exists, (void*)identifier );

    if ( variable == NULL )
	{
	    variable = create_variable_struct( identifier, t );
	    mylist_insert( &(function->variables), variable );
	}

    read_type( TREE_TYPE(t), function );

    return variable;
}

static t_myproof_variable *read_operand( tree t, t_myproof_function *function )
{
    t_myproof_variable *variable = NULL;
    enum tree_code tc;

    tc = TREE_CODE( t );

    switch ( tc )
	{
	case VAR_DECL:
	case PARM_DECL:
	case CONST_DECL:
	    variable = read_data( t, function );
	    break;

	case ARRAY_REF:
	    printf("variable pass array_ref\n");

	    read_operand( TREE_OPERAND(t,0), function ); /* array base */
	    variable = read_operand( TREE_OPERAND(t,1), function ); /* array index */

	    /* if ( store == true ) */
	    /* 	{ */
	    /* 	    function->nstore++; */
	    /* 	} */
	    /* else */
	    /* 	{ */
	    /* 	    function->nload++; */
	    /* 	} */

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
	    variable = read_data( SSA_NAME_VAR(t), function );
	    break;

	case COND_EXPR:
	case TARGET_MEM_REF:
	    break;

	default:
	    fprintf( stderr, "myproof: read_operand(): unhandled \'%s\'\n", tree_code_name[tc] );
	    gcc_unreachable( );
	}

    return variable;
}

static void read_stmt( gimple g, t_myproof_function *function )
{
    unsigned int i;
    enum gimple_code gc;

    gc = gimple_code( g );

    switch ( gc )
	{
	case GIMPLE_ASSIGN:
	    {
		t_myproof_variable *op1 = read_operand( gimple_op(g,1), function ); /* op1 */
		if ( op1 )
		    {
			op1->visited++;
		    }

		if ( gimple_num_ops(g) > 2 ) /* op2 */
		    {
			t_myproof_variable *op2 = read_operand( gimple_op(g,2), function );
			if ( op2 )
			    {
				op2->visited++;
			    }
		    }

		t_myproof_variable *opdef = read_operand( gimple_op(g,0), function ); /* op def */
		if ( opdef )
		    {
			opdef->modified++;
		    }
	    }
	    break;

	case GIMPLE_CALL:
	    for ( i = 0; i < gimple_call_num_args(g); ++i )
		{
		    read_operand( gimple_call_arg(g,i), function );
		}

	    if ( gimple_call_lhs(g) != NULL_TREE )
		{
		    read_operand( gimple_call_lhs(g), function );
		}
	    break;

	case GIMPLE_COND:
	    read_operand( gimple_cond_lhs(g), function ); /* op1 */
	    read_operand( gimple_cond_rhs(g), function ); /* op2 */
	    break;

	case GIMPLE_RETURN:
	    if ( gimple_return_retval(g) != NULL_TREE )
		{
		    read_operand( gimple_return_retval(g), function );
		}
	    break;

	case GIMPLE_DEBUG: break;

	default:
	    fprintf( stderr, "myproof: read_stmt(): unhandled \'%s\'\n", gimple_code_name[gc] );
	    gcc_unreachable ( );
	}
}

static void read_loop( struct loop *l, t_myproof_function *function )
{
    struct loop *inner;

    for ( inner = l->inner; inner != NULL; inner = inner->next )
	{
	    read_loop( inner, function );
	}
}

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

unsigned int pass_variable()
{
    warning(0, "%<%s%>", context);

    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    t_myproof_function *function = mylist_find( g_myproof_pass->functions, function_exists, (void*)identifier );

    if ( function == NULL )
	{
	    fprintf( stderr, "myproof: pass_variable: unhandled \'%s\' function\n", identifier );
	    return 0;
	}

    basic_block bb;
    gimple_stmt_iterator gsi;

    FOR_EACH_BB( bb )
    {
	for ( gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi) )
	    {
		read_stmt( gsi_stmt(gsi), function );
	    }
    }

    if ( cfun->x_current_loops != NULL )
	{
	    read_loop( cfun->x_current_loops->tree_root, function );
	}

    return 0;
}
