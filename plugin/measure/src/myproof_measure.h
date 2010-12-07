#ifndef __MYPROOF_MEASURE_H__
# define __MYPROOF_MEASURE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

#include "mylist.h"

    /*
    ** defines
    */

#define MYPROOF_MEASURE_NAME_SIZE 128

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

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYPROOF_MEASURE_H__ */
