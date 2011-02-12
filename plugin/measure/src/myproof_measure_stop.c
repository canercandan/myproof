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

#include "myproof_measure.h"

static unsigned int ncall = 0;

void myproof_measure_stop()
{
    t_myproof_measure_function *function = mylist_pop( &g_myproof_measure_functions );
    if ( function == NULL ) { return; }
    unsigned long stop = myproof_measure_rdtsc();
    //fprintf(g_myproof_measure_output, MYPROOF_MEASURE_FORMAT, ncall, function->name, function->start, stop);
    fprintf(g_myproof_measure_output, MYPROOF_MEASURE_FORMAT, function->name, function->start, stop);
    printf(MYPROOF_MEASURE_FORMAT_EXTENDED, ncall, function->name, function->start, stop, stop - function->start);
    ncall++;

    if ( g_myproof_measure_functions == NULL )
	{
	    fclose( g_myproof_measure_output );
	}
}
