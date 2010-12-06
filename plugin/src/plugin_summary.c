#include "myproof.h"

t_mylist_res print_variables( void *data )
{
    t_myproof_variable *variable = data;
    printf("  - variable { name: %8s, size: %2d, IR: %2d, IW: %2d, FR: %2d, FW: %2d, visited: %2d, modified: %2d }\n", variable->name, variable->size, variable->IR, variable->IW, variable->FR, variable->FW, variable->visited, variable->modified);
    return MYLIST_R_CONTINUE;
}

t_mylist_res print_functions( void *data )
{
    t_myproof_function *function = data;
    printf("* function: %s\n", function->name);
    mylist_all( function->variables, print_variables );
    return MYLIST_R_CONTINUE;
}

t_mylist_res print_instrumente_functions( void *data )
{
    t_myproof_instrumente_function *function = data;
    printf("* instrumented function: %s\n", function->name);
    return MYLIST_R_CONTINUE;
}

void plugin_summary( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    t_myproof *myproof = user_data;
    printf("** Summary **\n");
    mylist_all( myproof->functions, print_functions );
    mylist_all( myproof->instrumente_functions, print_instrumente_functions );
}
