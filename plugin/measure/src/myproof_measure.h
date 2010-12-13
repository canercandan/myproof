#ifndef __MYPROOF_MEASURE_H__
# define __MYPROOF_MEASURE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

#include <stdio.h>
#include "mylist.h"

    /*
    ** defines
    */

#define MYPROOF_MEASURE_NAME_SIZE 128
#define MYPROOF_MEASURE_OUTPUT "myproof.out"
    //#define MYPROOF_MEASURE_FORMAT "Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu\n"
#define MYPROOF_MEASURE_FORMAT "appel à la fonction %s entrée cycle %lu sortie cycle %lu\n"
#define MYPROOF_MEASURE_FORMAT_EXTENDED "Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu coût %lu\n"

    /*
    ** type definitions
    */

    typedef struct
    {
	char name[MYPROOF_MEASURE_NAME_SIZE];
	unsigned long start;
    } t_myproof_measure_function;

    /*
    ** functions
    */

    void myproof_measure_start( const char *fname );
    void myproof_measure_stop();

    inline unsigned long myproof_measure_rdtsc();

    /*
    ** global variables
    */

    extern t_mylist *g_myproof_measure_functions;
    extern FILE *g_myproof_measure_output;

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYPROOF_MEASURE_H__ */
