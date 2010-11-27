/* mylist_delete.c --- 
 * 
 * Filename: mylist_delete.c
 * Description: 
 * Author: Caner Candan
 * Maintainer: 
 * Created: Wed Dec 31 10:40:15 2008 (+0200)
 * Version: 
 * Last-Updated: Mon Jan  5 10:34:04 2009 (+0200)
 *           By: Caner Candan
 *     Update #: 54
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

static t_mylist_res callback(t_mylist *t, void *data)
{
    t_mylist *tmp;

    if (t->next == NULL)
	return (MYLIST_R_END);
    if (t->next->data != data)
	return (MYLIST_R_CONTINUE);
    tmp = t->next->next;
    free(t->next);
    t->next = tmp;
    return (MYLIST_R_FOUND);
}

void mylist_delete(t_mylist **t, void *data)
{
    if ((*t) == NULL)
	return;
    if ((*t)->data == data)
	{
	    (void)mylist_pop(t);
	    return;
	}
    mylist_all_struct(*t, callback, data);
}

/* mylist_delete.c ends here */
