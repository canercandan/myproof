#include "myproof.h"
#include "plugin-version.h" // to check plugin version

int plugin_is_GPL_compatible; // needed by gnu plugin api

static t_myproof *create_myproof_struct( const char *plugin_name )
{
    t_myproof *myproof = xmalloc( sizeof(*myproof) );
    strcpy( myproof->dump_file_name, plugin_name );
    mylist_init( &(myproof->functions) );
    mylist_init( &(myproof->instrumente_functions) );
    myproof->num_all_ops = 0;
    return myproof;
}

int plugin_init( struct plugin_name_args *info,
		 struct plugin_gcc_version *version )
{
    if (!plugin_default_version_check(version, &gcc_version)) { return 1; }

    t_myproof *myproof = create_myproof_struct( info->base_name );

    plugin_params(info, myproof);

    register_callback(info->base_name, PLUGIN_PRAGMAS, plugin_pragma, myproof);
    register_callback(info->base_name, PLUGIN_FINISH, plugin_ends, myproof);
    register_callback(info->base_name, PLUGIN_FINISH_UNIT, plugin_summary, myproof);
    register_callback(info->base_name, PLUGIN_FINISH_UNIT, plugin_error, myproof);

    plugin_pass(info, myproof);

    return 0;
}
