#include <stdlib.h>
#include <string.h>
#include "myproof.h"

t_mylist *g_functions = NULL;

void start( const char *fname )
{
    t_function *function = malloc(sizeof(*function));
    strcpy( function->name, fname );
    function->start = get_rdtsc();
    mylist_push( &g_functions, function );
}
