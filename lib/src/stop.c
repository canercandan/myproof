#include <stdio.h>
#include "myproof.h"

static unsigned int ncall = 0;

void stop()
{
    t_function *function = mylist_pop( &g_functions );
    printf("Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu\n", ncall, function->name, function->start, get_rdtsc());
    ncall++;
}
