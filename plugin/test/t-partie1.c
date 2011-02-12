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

#include <stdlib.h>

#pragma myproof instrumente (toto, main)

#define N 100000
#define M N

static int tab[N] = {1};

int toto (int *tab)
{
    int sum = tab[0] + tab[1] + tab[2];
    int i;

    for (i=0; i<N; i++)
    	{
    	    sum += tab[i] + 3 * tab[i+1];
    	    tab[i+1] = sum;
    	}

    sum += tab[0] + tab[1];

    for (i=0; i<M; i++)
    	{
    	    sum += tab[i] + 3 * tab[i+1] + 4 * tab[i-1];
    	    tab[i+1] = sum;
    	    tab[i+2] = 2*sum;
    	    tab[i+3] = 3*sum;
    	}
}

int main()
{
    toto(tab);
    return 0;
}
