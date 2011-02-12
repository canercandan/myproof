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
