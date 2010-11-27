// mylist.h --- 
// 
// Filename: mylist.h
// Description: 
// Author: Caner Candan
// Maintainer: 
// Created: Sun Dec 28 16:25:40 2008 (+0200)
// Version: 
// Last-Updated: Mon Jan  5 11:05:02 2009 (+0200)
//           By: Caner Candan
//     Update #: 69
// URL: 
// Keywords: 
// Compatibility: 
// 
// 

// Commentary: 
// 
// 
// 
// 

// Change log:
// 
// 
// 
// 
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
// 

// Code:

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

    int mylist_count(t_mylist*);

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYLIST_H__ */

// 
// mylist.h ends here
