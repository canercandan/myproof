/* mylist_pop.c --- 
 * 
 * Filename: mylist_pop.c
 * Description: 
 * Author: Caner Candan
 * Maintainer: 
 * Created: Sun Dec 28 16:53:01 2008 (+0200)
 * Version: 
 * Last-Updated: Sun Dec 28 16:55:21 2008 (+0200)
 *           By: Caner Candan
 *     Update #: 6
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

#include <stdlib.h>
#include "mylist.h"

void *mylist_pop(t_mylist **t)
{
    t_mylist *tmp;
    void *data;

    if ((*t) == NULL)
	return (NULL);
    tmp = (*t)->next;
    data = (*t)->data;
    free(*t);
    *t = tmp;
    return (data);
}

/* mylist_pop.c ends here */
