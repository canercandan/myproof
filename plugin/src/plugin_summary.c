#include "myproof.h"

static t_mylist_res print_variables( void *data )
{
    t_myproof_variable *variable = data;
    printf("  - variable { name: %8s, size: %2d, visited: %2d, modified: %2d }\n", variable->name, variable->size, variable->visited, variable->modified);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_basicblocks( void *data )
{
    t_myproof_basicblock *basicblock = data;
    if ( basicblock->loop == NULL )
	{
	    printf("  * BB { index: %u, nload: %u, nstore: %u }\n", basicblock->index, basicblock->nload, basicblock->nstore);
	}
    else
	{
	    printf("  * BB { index: %u, nload: %u, nstore: %u, niteration: %u }\n", basicblock->index, basicblock->nload, basicblock->nstore, basicblock->loop->niteration);
	}
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_functions( void *data )
{
    t_myproof_function *function = data;
    printf("* function: %s\n", function->name);
    mylist_all( function->variables, print_variables );
    mylist_all( function->basicblocks, print_basicblocks );
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_instrumente_functions( void *data )
{
    t_myproof_instrumente_function *function = data;
    printf("* instrumented function: %s\n", function->name);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_part1_basicblocks( void *data, void *user_data )
{
    t_myproof_basicblock *bb = data;
    FILE *output = user_data;
    if ( bb->loop == NULL )
	{
	    if ( bb->nload > 0 ) { fprintf(output, "%u load\n", bb->nload); }
	    if ( bb->nstore > 0 ) { fprintf(output, "%u store\n", bb->nstore); }
	}
    else
	{
	    if ( bb->nload > 0 ) { fprintf(output, "%u * %u load\n", bb->nload, bb->loop->niteration); }
	    if ( bb->nstore > 0 ) { fprintf(output, "%u * %u store\n", bb->nstore, bb->loop->niteration); }
	}
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_part1_functions( void *data, void *user_data )
{
    t_myproof_function *function = data;
    FILE *output = user_data;
    fprintf(output, "fonction %s\n", function->name);
    mylist_all_data( function->basicblocks, print_part1_basicblocks, (void*)output);
    return MYLIST_R_CONTINUE;
}

static void print_part1( t_myproof *myproof )
{
    FILE *output = fopen( myproof->dump_file_name, "w" );
    mylist_all_data( myproof->functions, print_part1_functions, (void*)output );
    fclose( output );
}

void plugin_summary( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    t_myproof *myproof = user_data;
    printf("** Summary **\n");
    mylist_all( myproof->functions, print_functions );
    mylist_all( myproof->instrumente_functions, print_instrumente_functions );
    print_part1( myproof );
}
