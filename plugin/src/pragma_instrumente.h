#ifndef __PRAGMA_INSTRUMENTE_H__
# define __PRAGMA_INSTRUMENTE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

#include "myproof.h"

    /*
    ** defines
    */

    /*
    ** type definitions
    */

    typedef void (*t_instrumente_handler)( cpp_reader*, tree );

    typedef struct
    {
	enum cpp_ttype type;
	t_instrumente_handler handler;
    } t_instrumente_token;

    /*
    ** functions
    */

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__PRAGMA_INSTRUMENTE_H__ */
