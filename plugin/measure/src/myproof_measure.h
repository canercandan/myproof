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

#ifndef __MYPROOF_MEASURE_H__
# define __MYPROOF_MEASURE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

#include <stdio.h>
#include "mylist.h"

    /*
    ** defines
    */

#define MYPROOF_MEASURE_NAME_SIZE 128
#define MYPROOF_MEASURE_OUTPUT "myproof.out"
    //#define MYPROOF_MEASURE_FORMAT "Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu\n"
#define MYPROOF_MEASURE_FORMAT "appel à la fonction '%s' entree cycle %lu sortie cycle %lu\n"
#define MYPROOF_MEASURE_FORMAT_EXTENDED "Appel %u à la fonction %s entrée cycle %lu sortie cycle %lu coût %lu\n"

    /*
    ** type definitions
    */

    typedef struct
    {
	char name[MYPROOF_MEASURE_NAME_SIZE];
	unsigned long start;
    } t_myproof_measure_function;

    /*
    ** functions
    */

    void myproof_measure_start( const char *fname );
    void myproof_measure_stop();

    inline unsigned long myproof_measure_rdtsc();

    /*
    ** global variables
    */

    extern t_mylist *g_myproof_measure_functions;
    extern FILE *g_myproof_measure_output;

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYPROOF_MEASURE_H__ */
