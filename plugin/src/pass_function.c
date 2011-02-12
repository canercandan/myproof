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

static const char *context = "pass function";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_myproof_function *create_function_struct( const char *name )
{
    t_myproof_function *function = xmalloc( sizeof(*function) );
    strcpy( function->name, name );
    mylist_init( &(function->variables) );
    mylist_init( &(function->basicblocks) );
    mylist_init( &(function->edges) );
    return function;
}

unsigned int pass_function()
{
    warning(0, "%<%s%>", context);

    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    if ( mylist_find( g_myproof_pass->functions, function_exists, (void*)identifier ) == NULL )
	{
	    mylist_insert( &(g_myproof_pass->functions), create_function_struct( identifier ) );
	}

    return 0;
}
