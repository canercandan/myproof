#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "myproof_measure.h"

t_mylist *g_myproof_measure_functions = NULL;
FILE *g_myproof_measure_output = NULL;

static unsigned int first = 1;

void myproof_measure_start( const char *fname )
{
    if ( first == 1 )
	{
	    mylist_init( &g_myproof_measure_functions );
	    unlink( MYPROOF_MEASURE_OUTPUT );
	    first = 0;
	}

    if ( g_myproof_measure_functions == NULL )
	{
	    g_myproof_measure_output = fopen( MYPROOF_MEASURE_OUTPUT, "a" );
	}

    t_myproof_measure_function *function = malloc(sizeof(*function));
    strcpy( function->name, fname );
    function->start = myproof_measure_rdtsc();
    mylist_push( &g_myproof_measure_functions, function );
}
