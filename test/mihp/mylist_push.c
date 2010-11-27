/* mylist_push.c --- 
 * 
 * Filename: mylist_push.c
 * Description: 
 * Author: Caner Candan
 *- Maintainer: 
 * Created: Sun Dec 28 16:52:53 2008 (+0200)
 * Version: 
 * Last-Updated: Wed Dec 31 10:38:14 2008 (+0200)
 *           By: Caner Candan
 *     Update #: 3
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

void mylist_push(t_mylist **t, void *data)
{
    t_mylist *new;

    new = xmalloc(sizeof(*new));
    new->data = data;
    new->next = *t;
    *t = new;
}

/* mylist_push.c ends here */
