#include "myproof.h"

// needed by gnu plugin api
int plugin_is_GPL_compatible;
extern struct opt_pass *current_pass;
// needed by gnu plugin api end

static bool our_gate (void) { return true; }
static bool our_main ( void ) { return true; } //mihp_pass();

static struct opt_pass our_pass = { GIMPLE_PASS,
				    "myproof's spy",
				    our_gate,
				    (void*)our_main,
				    NULL, NULL,
				    0, 0, 0, 0, 0, 0, 0 };

static t_myproof *create_myproof_struct()
{
    t_myproof *myproof = myproof = xmalloc( sizeof(*myproof) );
    mylist_init( &(myproof->variables) );
    myproof->num_all_ops = 0;
    return myproof;
}

int plugin_init( struct plugin_name_args *info,
		 struct plugin_gcc_version *version )
{
    if (!plugin_default_version_check (version, &gcc_version)) { return 1; }

    t_myproof *myproof = create_myproof_struct();

    struct register_pass_info newpass = { &our_pass, "mudflap2", 0, PASS_POS_INSERT_AFTER };

    register_callback (info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &newpass);
    register_callback (info->base_name, PLUGIN_FINISH, plugin_ends, myproof);
    register_callback (info->base_name, PLUGIN_FINISH_UNIT, plugin_summary, myproof);
    register_callback (info->base_name, PLUGIN_PRAGMAS, plugin_pragma, myproof);
    //register_callback (info->base_name, PLUGIN_ALL_PASSES_START, mihp_read_loop, NULL);

    return 0;
}
