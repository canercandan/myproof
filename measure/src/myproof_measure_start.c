#include <stdlib.h>
#include <string.h>
#include "myproof_measure.h"

t_mylist *g_myproof_measure_functions = NULL;

void myproof_measure_start( const char *fname )
{
    t_myproof_measure_function *function = malloc(sizeof(*function));
    strcpy( function->name, fname );
    function->start = myproof_measure_rdtsc();
    mylist_push( &g_myproof_measure_functions, function );
}
