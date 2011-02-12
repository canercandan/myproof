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

static const char *context = "pass basicblock";

static t_myproof_basicblock *create_basicblock_struct( const size_t index )
{
    t_myproof_basicblock *basicblock = xmalloc( sizeof(*basicblock) );
    basicblock->index = index;
    basicblock->nload = 0;
    basicblock->nstore = 0;
    basicblock->loop = NULL;
    return basicblock;
}

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static t_mylist_res basicblock_exists( void *data, void *user_data )
{
    t_myproof_basicblock* basicblock = data;
    return ( basicblock->index == (size_t)user_data ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

unsigned int pass_basicblock()
{
    warning(0, "%<%s%>", context);

    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(cfun->decl));

    t_myproof_function *function = mylist_find( g_myproof_pass->functions, function_exists, (void*)identifier );

    if ( function == NULL )
	{
	    fprintf( stderr, "myproof: pass_basicblock: unhandled \'%s\' function\n", identifier );
	    return 0;
	}

    basic_block bb;

    FOR_EACH_BB( bb )
    {
    	size_t index = bb->index;

    	t_myproof_basicblock *basicblock = mylist_find( function->basicblocks, basicblock_exists, (void*)index );

    	if ( basicblock == NULL )
    	    {
    		basicblock = create_basicblock_struct( index );
    		mylist_insert( &(function->basicblocks), basicblock );
    	    }
    }

    return 0;
}
