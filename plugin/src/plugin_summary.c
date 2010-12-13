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

static t_mylist_res print_edges( void *data )
{
    t_myproof_edge *edge = data;
    printf("  %% edge { bb_src: %2u, bb_dst: %2u }\n", edge->src->index, ( edge->dst == NULL ) ? 1 : edge->dst->index);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_functions( void *data )
{
    t_myproof_function *function = data;
    printf("* function: %s\n", function->name);
    mylist_all( function->variables, print_variables );
    mylist_all( function->basicblocks, print_basicblocks );
    mylist_all( function->edges, print_edges );
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
    fprintf(output, "fonction '%s'\n", function->name);
    mylist_all_data( function->basicblocks, print_part1_basicblocks, (void*)output);
    return MYLIST_R_CONTINUE;
}

static void print_part1( t_myproof *myproof )
{
    char filename[MYPROOF_NAME_SIZE];
    sprintf( filename, "%s.static", myproof->dump_file_name );
    FILE *output = fopen( filename, "w" );
    mylist_all_data( myproof->functions, print_part1_functions, (void*)output );
    fclose( output );
}

static t_mylist_res print_cfg_edges( void *data, void *user_data )
{
    t_myproof_edge *edge = data;
    void **args = user_data;
    FILE *output = args[0];
    size_t *fnum = args[1];
    fprintf(output, "\t\t\"F%u_BB%u\" -> \"F%u_BB%u\";\n", *fnum, edge->src->index, *fnum, (edge->dst == NULL) ? 1 : edge->dst->index);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res print_cfg_functions( void *data, void *user_data )
{
    t_myproof_function *function = data;
    void **args = user_data;
    FILE *output = args[0];
    size_t *fnum = args[1];
    fprintf(output, "\tsubgraph \"%s\" {\n\t\tlabel=\"%s\";\n", function->name, function->name);
    //fprintf(output, "\t\t\"F%u_BB1\" -> \"F%u_BB2\";\n", *fnum, *fnum);
    mylist_all_data( function->edges, print_cfg_edges, (void*)args);
    fprintf(output, "\t}\n");
    (*fnum)++;
    return MYLIST_R_CONTINUE;
}

static void print_cfg( t_myproof *myproof )
{
    size_t fnum = 0;
    char filename[MYPROOF_NAME_SIZE];
    sprintf( filename, "%s.cfg", myproof->dump_file_name );
    FILE *output = fopen( filename, "w" );
    const void *args[2] = { output, &fnum };
    fprintf(output, "digraph G {\n");
    mylist_all_data( myproof->functions, print_cfg_functions, (void*)args );
    fprintf(output, "}\n");
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
    print_cfg( myproof );
}
