#include "myproof.h"

t_myproof *g_myproof_pragma; // global to be available from pragmas

static const t_myproof_pragma_def pragmas[] = {
    { "instrumente", pragma_instrumente }
};

// we're registering our pragmas
void plugin_pragma( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    const int n_pragmas = sizeof(pragmas) / sizeof(*pragmas);
    int i;

    g_myproof_pragma = user_data;
    warning(0, "Loop to register pragmas");

    for (i = 0; i < n_pragmas; ++i)
	{
	    c_register_pragma(MYPROOF_PRAGMA_SPACE, pragmas[i].name, pragmas[i].handler);
	}
}
