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

static t_mylist_res removeall_node(void *data)
{
    free(data);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res removeall_basicblocks(void *data)
{
    t_myproof_basicblock *bb = data;
    if ( bb->loop == NULL ) { free(bb->loop); }
    free(data);
    return MYLIST_R_CONTINUE;
}

static t_mylist_res removeall_functions(void *data)
{
    t_myproof_function *function = data;
    mylist_all(function->variables, removeall_node);
    mylist_all(function->edges, removeall_node);
    mylist_all(function->basicblocks, removeall_basicblocks);
    free(data);
    return MYLIST_R_CONTINUE;
}

static void free_myproof_struct( t_myproof *myproof )
{
    mylist_all(myproof->functions, removeall_functions);
    mylist_all(myproof->instrumente_functions, removeall_node);
    free(myproof);
}

void plugin_ends( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    free_myproof_struct(user_data);
}
