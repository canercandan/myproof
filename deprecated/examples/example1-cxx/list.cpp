#include <cstdlib>

extern "C"
{

#include "gcc-plugin.h"
#include "coretypes.h"
#include "tree.h"
#include "tree-pass.h"
#include "diagnostic.h"
    //#include "cfgloop.h"

    int plugin_is_GPL_compatible;
    extern struct opt_pass *current_pass;

    extern void register_callback (const char*, int, plugin_callback_func, void*);

    void
    gate_callback (void *a, void *b)
    {
	if (errorcount || sorrycount)
	    return;
	fprintf (stderr, "Processing %s with %s\n", main_input_filename, current_pass->name);
    }

    int
    plugin_init (struct plugin_name_args *info, struct plugin_gcc_version *ver)
    {
	fprintf (stderr, "Starting %s\n", info->base_name);

	register_callback (info->base_name, PLUGIN_OVERRIDE_GATE, gate_callback, NULL);

	return 0;
    }

}
