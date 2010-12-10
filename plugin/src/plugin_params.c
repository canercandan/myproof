#include "myproof.h"

static void param_dump_file_name( t_myproof *myproof, char *value )
{
    strcpy( myproof->dump_file_name, value );
}

static const t_myproof_param_def params[] = {
    { "dump-file-name", param_dump_file_name }
};

void plugin_params( struct plugin_name_args *info, t_myproof *myproof )
{
    const int n_params = sizeof(params) / sizeof(*params);
    const char *plugin_name = info->base_name;
    int argc = info->argc;
    struct plugin_argument *argv = info->argv;
    int i, j;

    /*
     * Process the plugin arguments.
     */
    for (i = 0; i < argc; ++i)
	{
	    char *key = argv[i].key;
	    char *value = argv[i].value;
	    bool found = false;

	    if ( !value )
		{
		    error(G_("option '-fplugin-arg-%s-%s' requires a value"), plugin_name, key);
		    continue;
		}

	    for (j = 0; j < n_params; ++j)
		{
		    if ( !strcmp( key, params[j].key ) )
			{
			    params[j].handler( myproof, value );
			    found = true;
			    break;
			}
		}

	    if (!found) { error(G_("option '-fplugin-arg-%s-%s' doesnot exist"), plugin_name, key); }
	}
}
