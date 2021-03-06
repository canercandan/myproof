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

#include "myproof.h"

static const char *context = "pass instrumente";

static t_mylist_res function_exists( void *data, void *user_data )
{
    t_myproof_instrumente_function* function = data;
    return ( !strcmp(function->name, user_data) ) ? MYLIST_R_FOUND : MYLIST_R_CONTINUE;
}

static tree create_start_call( const char *arg )
{
    const char *fname = "myproof_measure_start";
    tree ftype = build_function_type_list( void_type_node /*return*/, ptr_type_node /*first arg*/, NULL_TREE );
    tree fndecl = build_fn_decl( fname, ftype );
    tree fs = build_string( strlen(arg) + 1, arg );

    DECL_ASSEMBLER_NAME( fndecl );
    TREE_TYPE( fs ) = build_array_type( char_type_node, build_index_type( build_int_cst( NULL_TREE, strlen( arg ) ) ) );
    TREE_CONSTANT( fs ) = 1;
    TREE_READONLY( fs ) = 1;
    TREE_STATIC( fs ) = 1;
    fs = build1( ADDR_EXPR, build_pointer_type( char_type_node ), fs );

    tree fargs = tree_cons( NULL_TREE, fs, NULL_TREE );

    return build_function_call_expr( UNKNOWN_LOCATION, fndecl, fargs );
}

static tree create_stop_call()
{
    const char *fname = "myproof_measure_stop";
    tree ftype = build_function_type_list( void_type_node /*return*/, NULL_TREE );
    tree fndecl = build_fn_decl( fname, ftype );

    DECL_ASSEMBLER_NAME( fndecl );

    return build_function_call_expr( UNKNOWN_LOCATION, fndecl, NULL_TREE );
}

unsigned int pass_instrumente()
{
    warning(0, "%<%s%>", context);

    tree t = cfun->decl;
    const char *identifier = IDENTIFIER_POINTER(DECL_NAME(t));

    if ( mylist_find( g_myproof_pass->instrumente_functions, function_exists, (void*)identifier ) == NULL )
	{
	    warning(0, "%<%s%> %s is not instrumentable", context, identifier);
	    return 0;
	}

    tree start_call = create_start_call( identifier );
    tree stop_call = create_stop_call( identifier );

    gimple start_stmt = gimple_build_call_from_tree( start_call );
    gimple stop_stmt = gimple_build_call_from_tree( stop_call );

    gimple_stmt_iterator gsi;

    gsi = gsi_start_bb( ENTRY_BLOCK_PTR->next_bb );
    gsi_insert_before( &gsi, start_stmt, GSI_SAME_STMT );

    gsi = gsi_start_bb( EXIT_BLOCK_PTR->prev_bb );
    gsi_insert_before( &gsi, stop_stmt, GSI_SAME_STMT );

    return 0;
}
