// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 3, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth
// Floor, Boston, MA 02110-1301, USA.
//
// Author: Caner Candan
//

#include <stdlib.h> // allocation
#include <string.h> // strcmp
#include "mylist.h"

t_mylist_res mylist_all(t_mylist *t, mylist_fct fct)
{
    t_mylist *next;
    t_mylist_res res;

    res = MYLIST_R_CONTINUE;
    while (t != NULL)
	{
	    next = t->next;
	    res = (*fct)(t->data);
	    if (res != MYLIST_R_CONTINUE)
		return (res);
	    t = next;
	}
    return (res);
}

t_mylist_res mylist_all_data(t_mylist *t, mylist_fct_data fct, void *data)
{
    t_mylist *next;
    t_mylist_res res;

    res = MYLIST_R_CONTINUE;
    while (t != NULL)
	{
	    next = t->next;
	    res = (*fct)(t->data, data);
	    if (res != MYLIST_R_CONTINUE)
		return (res);
	    t = next;
	}
    return (res);
}

t_mylist_res mylist_all_struct(t_mylist *t, mylist_fct_struct fct, void *data)
{
    t_mylist *next;
    t_mylist_res res;

    res = MYLIST_R_CONTINUE;
    while (t != NULL)
	{
	    next = t->next;
	    res = (*fct)(t, data);
	    if (res != MYLIST_R_CONTINUE)
		return (res);
	    t = next;
	}
    return (res);
}

void *mylist_find(t_mylist *t, mylist_fct_data fct, void *key)
{
    t_mylist *next;
    t_mylist_res res;

    res = MYLIST_R_CONTINUE;
    while (t != NULL)
	{
	    next = t->next;
	    res = (*fct)(t->data, key);
	    if (res != MYLIST_R_CONTINUE)
		return (t->data);
	    t = next;
	}
    return (NULL);
}

char *mylist_find_string(t_mylist *t, const char *key)
{
    t_mylist *next;

    while (t != NULL)
	{
	    next = t->next;
	    if (!strcmp(t->data, key))
		return (t->data);
	    t = next;
	}
    return (NULL);
}

int mylist_find_integer(t_mylist *t, int key)
{
    t_mylist *next;

    while (t != NULL)
	{
	    next = t->next;
	    if ((int)t->data == key)
		return ((int)t->data);
	    t = next;
	}
    return (-1);
}

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

static t_mylist_res mylist_delete_callback(t_mylist *t, void *data)
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
    mylist_all_struct(*t, mylist_delete_callback, data);
}

void mylist_free(t_mylist **t)
{
    while (mylist_pop(t) != NULL)
	{
#ifdef DEBUG
	    printf("*** free mylist\n");
#endif
	}
}

void mylist_init(t_mylist **t)
{
    (*t) = NULL;
}

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

void mylist_push(t_mylist **t, void *data)
{
    t_mylist *new;

    new = (t_mylist*)MALLOC(sizeof(*new));
    new->data = data;
    new->next = *t;
    *t = new;
}
