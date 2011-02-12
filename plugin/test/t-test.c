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

#include <math.h>

#pragma myproof instrumente (syracuse,main)

unsigned int
syracuse ( int N )
{
  int n = N;
  unsigned int a, b, c = 0;
  (void)a;
  (void)b;

  while ( n != 1 )
    {
      {
        int c;

        if ( n & 1 )
          c = 3*n + 1;
        else
          c = n / 2;

        n = c;
      }

      ++c;
    }

  return c;
}

int
main ( )
{
  unsigned int i;
  unsigned int syr;
  double v;
  double vp;
  float a;

  syr = 0;

  for ( i=1; i<=8; ++i )
    syr += syracuse ( i );

  v = pow ( syr, 2 );
  vp = v * M_PI;
  a = (float) sqrt ( vp );
  return (int) a;
}
