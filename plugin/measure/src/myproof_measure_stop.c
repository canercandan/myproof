#include <stdio.h>
#include "myproof_measure.h"

static unsigned int ncall = 0;

void myproof_measure_stop()
{
    t_myproof_measure_function *function = mylist_pop( &g_myproof_measure_functions );
    if ( function == NULL ) { return; }
    unsigned long stop = myproof_measure_rdtsc();
    printf("Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu coût %lu\n", ncall, function->name, function->start, stop, stop - function->start);
    ncall++;
}
