/* mylist_count.c --- 
 * 
 * Filename: mylist_count.c
 * Description: 
 * Author: Caner Candan
 * Maintainer: 
 * Created: Tue Dec 30 22:50:32 2008 (+0200)
 * Version: 
 * Last-Updated: Tue Dec 30 22:53:13 2008 (+0200)
 *           By: Caner Candan
 *     Update #: 12
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

#include "mylist.h"

int mylist_count(t_mylist *t)
{
    int	res;

    res = 0;
    while (t != NULL)
	{
	    res++;
	    t = t->next;
	}
    return (res);
}

/* mylist_count.c ends here */
