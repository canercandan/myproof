/* mylist_free.c --- 
 * 
 * Filename: mylist_free.c
 * Description: 
 * Author: Caner Candan
 * Maintainer: 
 * Created: Sun Dec 28 16:36:43 2008 (+0200)
 * Version: 
 * Last-Updated: Mon Jan  5 10:35:41 2009 (+0200)
 *           By: Caner Candan
 *     Update #: 26
 * URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include <stdio.h>
#include "mylist.h"

void mylist_free(t_mylist **t)
{
    while (mylist_pop(t) != NULL)
	{
#ifdef DEBUG
	    printf("*** free mylist\n");
#endif
	}
}

/* mylist_free.c ends here */
