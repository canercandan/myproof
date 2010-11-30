#include "myproof.h"

static t_mylist_res removeall_variable_list(void *data)
{
    free(data);
    return MYLIST_R_CONTINUE;
}

static void free_myproof_struct( t_myproof *myproof )
{
    mylist_all(myproof->variables, removeall_variable_list);
    mylist_free( &(myproof->variables) );
    free(myproof);
}

void plugin_ends( void *gcc_data, void *user_data ) //t_myproof*
{
    free_myproof_struct(user_data);
}
