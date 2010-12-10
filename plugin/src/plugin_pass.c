#include "myproof.h"

t_myproof *g_myproof_pass; // global to be available from passes

static const t_myproof_pass_def passes[] = {
    { GIMPLE_PASS, "function", pass_function, "parloops", 0, PASS_POS_INSERT_AFTER },
    { GIMPLE_PASS, "basicblock", pass_basicblock, "parloops", 0, PASS_POS_INSERT_AFTER },
    { GIMPLE_PASS, "loop", pass_loop, "parloops", 0, PASS_POS_INSERT_AFTER },
    { GIMPLE_PASS, "bb", pass_bb, "mudflap2", 0, PASS_POS_INSERT_AFTER },
    /* { GIMPLE_PASS, "edge", pass_edge, "mudflap2", 0, PASS_POS_INSERT_AFTER }, */
    { GIMPLE_PASS, "edge", pass_edge, "parloops", 0, PASS_POS_INSERT_AFTER },
    { GIMPLE_PASS, "variable", pass_variable, "mudflap2", 0, PASS_POS_INSERT_AFTER },
    { GIMPLE_PASS, "instrumente", pass_instrumente, "mudflap2", 0, PASS_POS_INSERT_AFTER },
    /* { GIMPLE_PASS, "verbose", pass_verbose, "mudflap2", 0, PASS_POS_INSERT_AFTER }, */
    { GIMPLE_PASS, "generic", pass_generic, "mudflap2", 0, PASS_POS_INSERT_AFTER },
};

static struct opt_pass option_passes[sizeof(passes) / sizeof(*passes)];

static bool dummy_gate(void) { return true; }

void plugin_pass( struct plugin_name_args *info, t_myproof *myproof )
{
    const int n_passes = sizeof(passes) / sizeof(*passes);
    int i;

    g_myproof_pass = myproof;
    warning(0, "Loop to register passes");

    for (i = n_passes - 1; i >= 0; --i)
	{
	    // common opt_pass structure configuration
	    option_passes[i].gate = dummy_gate;
	    option_passes[i].sub = NULL;
	    option_passes[i].next = NULL;
	    option_passes[i].static_pass_number = 0;
	    option_passes[i].tv_id = 0;
	    option_passes[i].properties_required = 0;
	    option_passes[i].properties_provided = 0;
	    option_passes[i].properties_destroyed = 0;
	    option_passes[i].todo_flags_start = 0;
	    option_passes[i].todo_flags_finish = 0;
	    // common opt_pass structure configuration end

	    option_passes[i].type = passes[i].pass;
	    option_passes[i].name = passes[i].name;
	    option_passes[i].execute = passes[i].execute;

	    struct register_pass_info pass_info = { &(option_passes[i]), passes[i].reference_pass_name, passes[i].ref_pass_instance_number, passes[i].pos_op };

	    register_callback( info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &pass_info );
	}
}
