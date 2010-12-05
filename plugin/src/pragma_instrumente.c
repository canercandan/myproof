#include "pragma_instrumente.h"

static const char *context = "#pragma myproof instrumente";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_instrumente_function* function = data;
    return ( (!strcmp(function->name, user_data)) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE );
}

/* static t_mylist_res list_functions( void *data ) */
/* { */
/*     t_myproof_instrumente_function* function = data; */
/*     printf("current function name: %s\n", function->name); */
/*     return MYLIST_R_CONTINUE; */
/* } */

static void create_function( tree t )
{
    const char *identifier = IDENTIFIER_POINTER(t); // look at gimple parsing
    t_mylist *functions = g_myproof_pragma->instrumente_functions;
    if ( mylist_all_data( functions, function_exists, (void*)identifier ) == MYLIST_R_CONTINUE )
	{
	    t_myproof_instrumente_function *function = xmalloc( sizeof( *function ) );
	    strcpy( function->name, identifier );
	    mylist_push( &( g_myproof_pragma->instrumente_functions), (void*)function );
	    /* printf("function \'%s\' added\n", identifier); */
	    /* mylist_all( g_myproof_pragma->instrumente_functions, list_functions ); */
	}
}

static void single_function( cpp_reader *parse_in, tree t )
{
    (void)parse_in;
    printf("single_function\n");

    tree next_tree = 0;
    if ( pragma_lex( &next_tree ) != CPP_EOF )
	{
	    warning(OPT_Wpragmas, "excepted end after %<%s%> function_name", context);
	    return;
	}

    create_function( t );
}

static void multiple_function( cpp_reader *parse_in, tree t )
{
    (void)parse_in;
    printf("multiple_function\n");
    enum cpp_ttype token = pragma_lex( &t ); // parse: "( TOKEN"

    if ( token != CPP_NAME ) // found: "( NOTID"
	{
	    warning(OPT_Wpragmas, "excepted one or several functions after %<%s%>", context);
	    return;
	}

    // found: "( ID"

    do
	{
	    create_function( t );
	    token = pragma_lex( &t ); // parse: "( ID TOKEN"
	    if ( token == CPP_CLOSE_PAREN ) { return; }  // found: "( ID )"
	    if ( token == CPP_COMMA ) // found: "( ID ,"
		{
		    token = pragma_lex( &t );
		    continue;
		}

	    // found: "( ID NOTCOMMA"

	    warning(OPT_Wpragmas, "excepted comma after %<%s%> ( function_name", context);
	    return;
	}
    while ( token == CPP_NAME ); // found: "( ID, ID"

    // found: "( ID [...] NOTCLOSEPARENTHESE"

    warning(OPT_Wpragmas, "excepted close parenthese after %<%s%> ( function_name", context);
}

static const t_instrumente_token tokens[] = {
    { CPP_NAME, single_function },
    { CPP_OPEN_PAREN, multiple_function }
};

void pragma_instrumente( cpp_reader *parse_in )
{
    warning(OPT_Wpragmas, "%<%s%>", context);

    tree t = 0;
    enum cpp_ttype token = pragma_lex( &t );

    const int n_tokens = sizeof(tokens) / sizeof(*tokens);
    int i;
    for (i = 0; i < n_tokens; ++i)
	{
	    if ( token == tokens[i].type ) // look at cpplib.h
		{
		    tokens[i].handler( parse_in, t );
		    return;
		}
	}

    warning(OPT_Wpragmas, "%<%s%> unknown token instead function names", context);
}
