#include <stdio.h>
#include "myproof_measure.h"

static unsigned int ncall = 0;

void myproof_measure_stop()
{
    t_myproof_measure_function *function = mylist_pop( &g_myproof_measure_functions );
    if ( function == NULL ) { return; }
    //printf("coco\n");
    printf("Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu\n", ncall, function->name, function->start, myproof_measure_rdtsc());
    ncall++;
}
