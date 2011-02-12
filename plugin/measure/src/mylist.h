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
 * Authors: Caner Candan <caner@candan.fr>, http://caner.candan.fr
 */

#ifndef __MYLIST_H__
# define __MYLIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** defines
    */

# ifndef NULL
#  define NULL	(void*)0
# endif /* !NULL */

#define MALLOC malloc

    /*
    ** type definitions
    */

    typedef enum e_mylist_res {MYLIST_R_FOUND,
			       MYLIST_R_CONTINUE,
			       MYLIST_R_END,
			       MYLIST_R_ERROR} t_mylist_res;

    struct s_mylist;

    typedef t_mylist_res (*mylist_fct)(void *data_mylist);
    typedef t_mylist_res (*mylist_fct_data)(void *data_mylist, void *data);
    typedef t_mylist_res (*mylist_fct_struct)(struct s_mylist*, void *data);

    /*
    ** structures
    */

    typedef struct	s_mylist
    {
	void		*data;
	struct s_mylist	*next;
    }			t_mylist;

    /*
    ** functions
    */

    void mylist_init(t_mylist**);
    void mylist_free(t_mylist**);

    void mylist_push(t_mylist**, void *data);
    void *mylist_pop(t_mylist**);

    void mylist_delete(t_mylist**, void *data);

    t_mylist_res mylist_all(t_mylist*, mylist_fct);
    t_mylist_res mylist_all_data(t_mylist*, mylist_fct_data, void *data);
    t_mylist_res mylist_all_struct(t_mylist*, mylist_fct_struct, void *data);

    void *mylist_find(t_mylist*, mylist_fct_data, void *key);
    char *mylist_find_string(t_mylist*, const char *key);
    int mylist_find_integer(t_mylist*, int key);

    int mylist_count(t_mylist*);

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYLIST_H__ */
