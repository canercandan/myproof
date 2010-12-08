#include "myproof.h"
#include "plugin-version.h" // to check plugin version

int plugin_is_GPL_compatible; // needed by gnu plugin api

static t_myproof *create_myproof_struct()
{
    t_myproof *myproof = xmalloc( sizeof(*myproof) );
    strcpy( myproof->dump_file_name, "instrumentation.txt" );
    mylist_init( &(myproof->functions) );
    mylist_init( &(myproof->instrumente_functions) );
    myproof->num_all_ops = 0;
    return myproof;
}

void parameters_parsing( struct plugin_name_args *info, t_myproof *myproof )
{
    const char *plugin_name = info->base_name;
    int argc = info->argc;
    struct plugin_argument *argv = info->argv;
    int i;

    /*
     * Process the plugin arguments. This plugin takes the following argument:
     * dump-file-name=<PASS_NAME>.
     */
    for (i = 0; i < argc; ++i)
	{
	    if ( !strcmp(argv[i].key, "dump-file-name") )
		{
		    if ( argv[i].value )
			{
			    strcpy( myproof->dump_file_name, argv[i].value );
			}
		    else
			{
			    warning(0, G_("option '-fplugin-arg-%s-dump-file-name' requires a file name"), plugin_name);
			}
		}
	}
}

int plugin_init( struct plugin_name_args *info,
		 struct plugin_gcc_version *version )
{
    if (!plugin_default_version_check(version, &gcc_version)) { return 1; }

    t_myproof *myproof = create_myproof_struct();

    parameters_parsing( info, myproof );

    register_callback(info->base_name, PLUGIN_PRAGMAS, plugin_pragma, myproof);
    register_callback(info->base_name, PLUGIN_FINISH, plugin_ends, myproof);
    register_callback(info->base_name, PLUGIN_FINISH_UNIT, plugin_summary, myproof);
    register_callback(info->base_name, PLUGIN_FINISH_UNIT, plugin_error, myproof);

    plugin_pass(info, myproof);

    return 0;
}
