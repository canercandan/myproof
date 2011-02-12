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

#ifndef __PRAGMA_INSTRUMENTE_H__
# define __PRAGMA_INSTRUMENTE_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

#include "myproof.h"

    /*
    ** defines
    */

    /*
    ** type definitions
    */

    typedef void (*t_instrumente_handler)( cpp_reader*, tree );

    typedef struct
    {
	enum cpp_ttype type;
	t_instrumente_handler handler;
    } t_instrumente_token;

    /*
    ** functions
    */

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__PRAGMA_INSTRUMENTE_H__ */
