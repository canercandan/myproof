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

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_mylist_res for_each_instrumente_function( void *data, void *user_data )
{
    t_myproof *myproof = user_data;
    t_myproof_instrumente_function* instrumente_function = data;
    if ( mylist_find( myproof->functions, function_exists, instrumente_function->name ) == NULL )
	{
	    fprintf( stderr, "%s does not exist\n", instrumente_function->name );
	}
    return MYLIST_R_CONTINUE;
}

void plugin_error( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    t_myproof *myproof = user_data;
    mylist_all_data( myproof->instrumente_functions, for_each_instrumente_function, myproof );
}
