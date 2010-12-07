#include "myproof_measure.h"

static unsigned int ncall = 0;

void myproof_measure_stop()
{
    t_myproof_measure_function *function = mylist_pop( &g_myproof_measure_functions );
    if ( function == NULL ) { return; }
    unsigned long stop = myproof_measure_rdtsc();
    fprintf(g_myproof_measure_output, MYPROOF_MEASURE_FORMAT, ncall, function->name, function->start, stop);
    printf(MYPROOF_MEASURE_FORMAT_EXTENDED, ncall, function->name, function->start, stop, stop - function->start);
    ncall++;

    if ( g_myproof_measure_functions == NULL )
	{
	    fclose( g_myproof_measure_output );
	}
}
