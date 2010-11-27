/*
 * mihp.c
 *
 * MIHP pass
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include "gcc-plugin.h"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "cfgloop.h"

#include "mylist.h"

    typedef struct { char name[128]; int size, IR, IW, FR, FW; } t_mydata;
    static t_mylist *mylist;

    static unsigned int num_all_ops;
    static unsigned int num_all_ops_write;

    int plugin_is_GPL_compatible;
    extern struct opt_pass *current_pass;

    extern void register_callback (const char*, int, plugin_callback_func, void*);

    /*
     * Here's some callbacks used by mylist
     */
    static t_mylist_res find_name_in_mylist( void *current_data_in_mylist, void *data )
    {
	t_mydata* my_current_data_in_mylist = current_data_in_mylist;
	t_mydata* mydata = data;

	return ( (strcmp(my_current_data_in_mylist->name, mydata->name) == 0) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE );
    }

    static t_mylist_res removeall_names_from_mylist(void *data)
    {
	free(data);
	return (MYLIST_R_CONTINUE);
    }

    /*
     * mihp_get_type_size
     */
    int mihp_get_type_size ( tree );

    int
    mihp_get_type_size ( tree t )
    {
	int ret=0;

	if ( TYPE_SIZE_UNIT(t) != NULL )
	    ret = TREE_INT_CST_LOW ( TYPE_SIZE_UNIT(t) );

	return ret;
    }


    static int mihp_get_size( tree t )
    {
	enum tree_code tc;

	tc = TREE_CODE ( t );

	int size = -1;

	switch ( tc )
	    {
	    case BOOLEAN_TYPE:
	    case INTEGER_TYPE:
	    case REAL_TYPE:
		size = mihp_get_type_size( t ); break;

	    default:
		fprintf ( stderr, "mihp: mihp_get_size(): %s is not handled\n", tree_code_name[tc] );
		gcc_unreachable ( );
	    }

	return size;
    }

    /*
     * MIHP pass
     */
    static void
    mihp_read_type ( tree t )
    {
	enum tree_code tc;

	tc = TREE_CODE ( t );

	switch ( tc )
	    {
	    case INTEGER_TYPE:
	    case BOOLEAN_TYPE:
	    case REAL_TYPE:
		/* scalar */

		/* printf( "mihp_get_size: %d\n", mihp_get_size( t ) ); */

		break;

	    default:
		fprintf ( stderr, "mihp: mihp_read_type(): %s is not handled\n", tree_code_name[tc] );
		gcc_unreachable ( );
	    }
    }

    static void
    mihp_read_data ( tree t )
    {
	if ( DECL_NAME(t) )
	    {
		if ( mylist_all_data( mylist, find_name_in_mylist, (void*)IDENTIFIER_POINTER(DECL_NAME(t)) ) == MYLIST_R_CONTINUE )
		    {
			t_mydata *mydata = xmalloc( sizeof(*mydata) );
			strcpy(mydata->name, IDENTIFIER_POINTER(DECL_NAME(t)));
			mylist_push( &mylist, mydata );
			printf ( "       data \'%s\'\n", IDENTIFIER_POINTER(DECL_NAME(t)) );
		    }
	    }
	else
	    {
		/* printf ( "       data \'%c_%u\'\n", (TREE_CODE(t)==CONST_DECL)?'C':'D', DECL_UID(t) ); */
	    }

	mihp_read_type ( TREE_TYPE(t) );
    }

    static void
    mihp_read_operand ( tree t )
    {
	enum tree_code tc;

	tc = TREE_CODE ( t );

	tree tree_new = TREE_TYPE( t );
	enum tree_code tc_new = TREE_CODE ( t );

	if ( tc_new == REAL_TYPE )
	    printf("this is a real variable\n");
	else if ( tc_new == INTEGER_TYPE )
	    printf("this is an integer variable\n");

	switch ( tc )
	    {
	    case VAR_DECL:
	    case PARM_DECL:
	    case CONST_DECL:
		mihp_read_data ( t );
		break;

	    case ARRAY_REF:
		mihp_read_operand ( TREE_OPERAND(t,0) ); /* array base */
		mihp_read_operand ( TREE_OPERAND(t,1) ); /* array index */
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
		mihp_read_data ( SSA_NAME_VAR(t) );
		break;

	    case COND_EXPR:
		break;

	    default:
		fprintf ( stderr, "mihp: mihp_read_operand(): unhandled \'%s\'\n", tree_code_name[tc] );
		gcc_unreachable ( );
	    }
    }

    static void
    mihp_read_stmt ( gimple g )
    {
	unsigned int i;
	enum gimple_code gc;

	gc = gimple_code ( g );

	/* debug_tree(type1); */

	//debug_tree(type1);

	switch ( gc )
	    {
	    case GIMPLE_ASSIGN:
	    case GIMPLE_CALL:
	    case GIMPLE_COND:
	    case GIMPLE_RETURN:
		num_all_ops += gimple_num_ops(g);
	    }

	switch ( gc )
	    {
	    case GIMPLE_ASSIGN:
		mihp_read_operand ( gimple_op(g,1) ); /* op1 */
		num_all_ops_write++;

		tree type1 = TREE_TYPE( gimple_op( g, 1 ) );
		int tc = TREE_CODE( type1 );

		if ( tc == REAL_TYPE )
		    {
			//debug_tree(type1);
			//fprintf ( stderr, "toto\n" );
			/* write(1, "toto\n", 5); */
			printf("this is a real variable\n");
		    }

		enum tree_code tr = gimple_assign_rhs_code(g);
		printf("Doing %s\n", tree_code_name[tr]);

		if ( gimple_num_ops(g) > 2 ) /* op2 */
		    {
			mihp_read_operand ( gimple_op(g,2) );
		    }

		mihp_read_operand ( gimple_op(g,0) ); /* op def */
		break;

	    case GIMPLE_CALL:
		for ( i=0; i<gimple_call_num_args(g); ++i )
		    mihp_read_operand ( gimple_call_arg(g,i) );

		if ( gimple_call_lhs(g) != NULL_TREE )
		    mihp_read_operand ( gimple_call_lhs(g) );
		break;

	    case GIMPLE_COND:
		mihp_read_operand ( gimple_cond_lhs(g) ); /* op1 */
		mihp_read_operand ( gimple_cond_rhs(g) ); /* op2 */
		break;

	    case GIMPLE_RETURN:
		if ( gimple_return_retval(g) != NULL_TREE )
		    mihp_read_operand ( gimple_return_retval(g) );
		break;

	    default:
		fprintf ( stderr, "mihp: mihp_read_stmt(): unhandled \'%s\'\n", gimple_code_name[gc] );
		gcc_unreachable ( );
	    }
    }

    static void
    mihp_read_loop ( struct loop *l )
    {
	struct loop *inner;

	fprintf ( stderr, " # loop %d", l->num );

	if ( double_int_zero_p(l->nb_iterations_estimate) ) /* double_int type defined in double-int.h */
	    fprintf ( stderr, " has unknown number of iterations" );
	else
	    fprintf ( stderr, " has %llu iterations", l->nb_iterations_estimate.low );

	fprintf ( stderr, "\n" );

	for ( inner=l->inner; inner!=NULL; inner=inner->next )
	    mihp_read_loop ( inner );
    }

    static unsigned int
    mihp_pass ( void )
    {
	basic_block bb;
	gimple_stmt_iterator gsi;

	fprintf ( stderr, "* MIHP on %s()\n", IDENTIFIER_POINTER(DECL_NAME(cfun->decl)) );

	FOR_EACH_BB ( bb )
	{
	    /* fprintf ( stderr, "  ** BB %d\n", bb->index ); */

	    for ( gsi=gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi) )
		{
		    /* print_gimple_stmt ( stdout, gsi_stmt(gsi), 0, 0 ); */
		    mihp_read_stmt ( gsi_stmt(gsi) );
		}

	}

	/* recursively read looops */
	if ( cfun->x_current_loops != NULL )
	    mihp_read_loop ( cfun->x_current_loops->tree_root );

	return 0;
    }

    static bool
    our_gate (void)
    {
	return true;
    }

    static bool
    our_main ( void )
    {
	return mihp_pass();
    }

    struct opt_pass our_pass = {
	GIMPLE_PASS,
	"MIHP pass",
	our_gate,
	our_main,
	NULL,
	NULL,
	0,
	0,
	0,
	0,
	0,
	0,
	0
    };

    static void plugin_ends( void *gcc_data, void *user_data )
    {
	mylist_all(mylist, removeall_names_from_mylist);
	mylist_free(&mylist);
	printf("number of reading operands: %d\n", num_all_ops - num_all_ops_write);
	printf("number of writing operands: %d\n", num_all_ops_write);
	printf("number of operands: %d\n", num_all_ops);
    }

    /* static void mihp_read_loop( void *gcc_data, void *user_data ) */
    /* { */
    /* 	printf("loop\n"); */
    /* } */

    int
    plugin_init (struct plugin_name_args *info, struct plugin_gcc_version *ver)
    {
	mylist_init(&mylist);
	num_all_ops = 0;
	num_all_ops_write = 0;

	struct register_pass_info newpass = {&our_pass, "mudflap2", 0, PASS_POS_INSERT_AFTER};

	register_callback (info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &newpass);
	register_callback (info->base_name, PLUGIN_FINISH, plugin_ends, NULL);
	//register_callback (info->base_name, PLUGIN_ALL_PASSES_START, mihp_read_loop, NULL);

	return 0;
    }

#ifdef __cplusplus
}
#endif /* !__cplusplus */
