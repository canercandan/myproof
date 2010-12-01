#include "myproof.h"

//g_myproof_pragma

static bool state = false;

static const char *context = "#pragma myproof instrumentation";

static void handle_on( cpp_reader *parse_in )
{
    warning(OPT_Wpragmas, "%<%s%> on", context);
    state = true;
}

static void handle_off( cpp_reader *parse_in )
{
    warning(OPT_Wpragmas, "%<%s%> off", context);
    state = false;
}

static void handle_state( cpp_reader *parse_in )
{
    warning(OPT_Wpragmas, "%<%s%> state", context);
    printf("state: %d\n", state);
}

static const t_myproof_pragma_def clauses[] = {
    { 0, "on", handle_on },
    { 1, "off", handle_off },
    { 2, "state", handle_state }
};

void pragma_instrumentation( cpp_reader *parse_in )
{
    tree x = 0;
    enum cpp_ttype token = pragma_lex( &x );

    if ( token != CPP_NAME ) // look at cpplib.h
	{
	    warning(OPT_Wpragmas, "%<%s%> unknown token type \"%d\" instead cpp_name", context, token);
	    return;
	}

    const char *identifier = IDENTIFIER_POINTER(x); // look at gimple parsing

    const int n_clauses = sizeof(clauses) / sizeof(*clauses);
    int i;
    for (i = 0; i < n_clauses; ++i)
	{
	    if ( !strcmp( identifier, clauses[i].name ) )
		{
		    clauses[i].handler( parse_in );
		    return;
		}
	}

    warning(OPT_Wpragmas, "%<%s%> unknown identifier value \"%s\".", context, identifier);
}
