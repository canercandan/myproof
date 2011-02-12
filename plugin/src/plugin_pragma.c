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

t_myproof *g_myproof_pragma; // global to be available from pragmas

static const t_myproof_pragma_def pragmas[] = {
    { "instrumente", pragma_instrumente }
};

// we're registering our pragmas
void plugin_pragma( void *gcc_data, void *user_data ) //t_myproof*
{
    (void)gcc_data;
    const int n_pragmas = sizeof(pragmas) / sizeof(*pragmas);
    int i;

    g_myproof_pragma = user_data;
    warning(0, "Loop to register pragmas");

    for (i = 0; i < n_pragmas; ++i)
	{
	    c_register_pragma(MYPROOF_PRAGMA_SPACE, pragmas[i].name, pragmas[i].handler);
	}
}
