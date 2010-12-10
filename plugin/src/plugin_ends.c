#include "myproof.h"

static t_mylist_res removeall_node(void *data)
{
    free(data);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res removeall_basicblocks(void *data)
{
    t_myproof_basicblock *bb = data;
    if ( bb->loop == NULL ) { free(bb->loop); }
    free(data);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res removeall_functions(void *data)
{
    t_myproof_function *function = data;
    mylist_all(function->variables, removeall_node);
    mylist_all(function->edges, removeall_node);
    mylist_all(function->basicblocks, removeall_basicblocks);
    free(data);
    return MYLIST_R_CONTINUE;
}

static void free_myproof_struct( t_myproof *myproof )
{
    mylist_all(myproof->functions, removeall_functions);
    mylist_all(myproof->instrumente_functions, removeall_node);
    free(myproof);
}

void plugin_ends( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    free_myproof_struct(user_data);
}
