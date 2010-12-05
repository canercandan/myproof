#include "myproof.h"

static const char *context = "pass instrumente";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_instrumente_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

unsigned int pass_instrumente()
{
    warning(0, "%<%s%>", context);

    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    if ( mylist_find( g_myproof_pass->instrumente_functions, function_exists, (void*)identifier ) == NULL )
	{
	    fprintf( stderr, "%s is not instrumentable\n", identifier );
	    return 0;
	}

    fprintf( stderr, "* MYPROOF on %s()\n", identifier );

    return 0;
}
