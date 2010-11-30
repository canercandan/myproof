#include "myproof.h"

static void handle_pragma_test( cpp_reader* parse_in )
{
    tree token = 0;

    pragma_lex( &token );

    if ( pragma_lex( &token ) != CPP_NAME )
	{
	    warning(OPT_Wpragmas, G_("\%<#pragma %s %s\%>  is not a name"), MYPROOF_PRAGMA_SPACE, "test");
	    return;
	}

    printf("Directive MYPROOF TEST reconnue.\n");
}

static const t_myproof_pragma_def pragmas[] = {
    { "test", MYPROOF_PRAGMA_TEST, handle_pragma_test }
};

// we're registering our pragmas
void plugin_pragma( void *gcc_data, void *user_data ) //t_myproof*
{
    warning (0, G_("Loop to register pragmas %s"), "TOTOT");
    const int n_pragmas = sizeof(pragmas) / sizeof(*pragmas);
    int i;
    for (i = 0; i < n_pragmas; ++i)
	{
	    c_register_pragma(MYPROOF_PRAGMA_SPACE, pragmas[i].name, pragmas[i].handler);
	}
}
