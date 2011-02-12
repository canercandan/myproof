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

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "myproof_measure.h"

t_mylist *g_myproof_measure_functions = NULL;
FILE *g_myproof_measure_output = NULL;

static unsigned int first = 1;

void myproof_measure_start( const char *fname )
{
    if ( first == 1 )
	{
	    mylist_init( &g_myproof_measure_functions );
	    unlink( MYPROOF_MEASURE_OUTPUT );
	    first = 0;
	}

    if ( g_myproof_measure_functions == NULL )
	{
	    g_myproof_measure_output = fopen( MYPROOF_MEASURE_OUTPUT, "a" );
	}

    t_myproof_measure_function *function = malloc(sizeof(*function));
    strcpy( function->name, fname );
    function->start = myproof_measure_rdtsc();
    mylist_push( &g_myproof_measure_functions, function );
}
