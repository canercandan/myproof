#include "myproof.h"

void plugin_summary( void *gcc_data, void *user_data ) //t_myproof*
{
    t_myproof *myproof = user_data;
    printf("number of operands: %d\n", myproof->num_all_ops);
}
