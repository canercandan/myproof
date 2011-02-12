// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; comment-column: 35; -*-

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * Authors:
 * Caner Candan <caner@candan.fr>, http://caner.candan.fr
 * Aurèle Mahéo <aurele.maheo@gmail.com>
 */

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
