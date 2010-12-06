#ifndef __MYPROOF_H__
# define __MYPROOF_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

#include <stdint.h>
#include "mylist.h"

    /*
    ** defines
    */

#define MYPROOF_NAME_SIZE 128

    /*
    ** type definitions
    */

    typedef struct
    {
	char name[MYPROOF_NAME_SIZE];
	unsigned long start;
    } t_function;

    /*
    ** functions
    */

    void start( const char *fname );
    void stop();

    inline unsigned long get_rdtsc() { __asm__ ("xor %eax, %eax; cpuid; rdtsc"); }

    /*
    ** global variables
    */

    extern t_mylist *g_functions;

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYPROOF_H__ */
