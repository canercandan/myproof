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

#ifndef __MYPROOF_H__
# define __MYPROOF_H__

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ** headers
    */

    // headers used for gcc plugin
#include "gcc-plugin.h"
    // headers used for gcc plugin end

    // header files needed by gcc gimple
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "tree.h"
#include "function.h"
#include "tree-flow.h"
#include "tree-pass.h"
#include "cfgloop.h"
#include "intl.h"
    // header files needed by gcc gimple end

    // headers used for pragma
#include "c-pragma.h"
#include "cpplib.h" // for enum cpp_ttype
    // headers used for pragma end

#include "toplev.h" // warning function
#include "diagnostic.h" // print_gimple_stmt definition

#include "mylist.h" // get a linked-list

    /*
    ** defines
    */

    #define MYPROOF_PRAGMA_SPACE "myproof"
    #define MYPROOF_NAME_SIZE 128

    /*
    ** type definitions
    */

    /* variable structure */
    typedef struct
    {
	char name[MYPROOF_NAME_SIZE];
	size_t size;
	size_t visited;
	size_t modified;
    } t_myproof_variable;

    /* loop structure */
    typedef struct
    {
	size_t niteration;
    } t_myproof_loop;

    /* basicblock structure */
    typedef struct
    {
	size_t index;
	size_t nload;
	size_t nstore;
	t_myproof_loop *loop;
    } t_myproof_basicblock;

    /* edge structure */
    typedef struct
    {
	t_myproof_basicblock *src;
	t_myproof_basicblock *dst;
    } t_myproof_edge;

    /* function structure */
    typedef struct
    {
	char name[MYPROOF_NAME_SIZE];
	t_mylist *variables;
	t_mylist *basicblocks;
	t_mylist *edges;
    } t_myproof_function;

    /* function instrumenting structure */
    typedef struct
    {
	char name[MYPROOF_NAME_SIZE];
    } t_myproof_instrumente_function;

    /* main structure */
    typedef struct
    {
	char dump_file_name[MYPROOF_NAME_SIZE];
	t_mylist *functions;
	t_mylist *instrumente_functions;
	size_t num_all_ops;
    } t_myproof;

    /* prototype function for pragmas */
    typedef void (*t_myproof_pragma_handler)( cpp_reader* );

    /* pragma definition structure */
    typedef struct
    {
	// unsigned int id;
	const char *name;
	t_myproof_pragma_handler handler;
    } t_myproof_pragma_def;

    /* prototype function for pragmas */
    typedef unsigned int (*t_myproof_pass_handler)();

    /* pass definition structure */
    typedef struct
    {
	// unsigned int id;
	unsigned int pass;
	const char *name;
	t_myproof_pass_handler execute;
	const char *reference_pass_name;
	unsigned int ref_pass_instance_number;
	unsigned int pos_op;
    } t_myproof_pass_def;

    /* prototype function for params */
    typedef void (*t_myproof_param_handler)( t_myproof*, char* );

    /* param definition structure */
    typedef struct
    {
	const char *key;
	t_myproof_param_handler handler;
    } t_myproof_param_def;

    /*
    ** functions
    */

    int plugin_init( struct plugin_name_args*, struct plugin_gcc_version* );
    void plugin_ends( void *gcc_data, void *user_data );
    void plugin_summary( void *gcc_data, void *user_data );
    void plugin_error( void *gcc_data, void *user_data );
    void plugin_pragma( void *gcc_data, void *user_data );
    void plugin_pass( struct plugin_name_args*, t_myproof* );
    void plugin_params( struct plugin_name_args*, t_myproof* );

    void pragma_instrumente( cpp_reader* );

    unsigned int pass_generic();
    unsigned int pass_verbose();
    unsigned int pass_instrumente();
    unsigned int pass_function();
    unsigned int pass_basicblock();
    unsigned int pass_loop();
    unsigned int pass_bb();
    unsigned int pass_edge();
    unsigned int pass_variable();

    /*
    ** global variables
    */

    extern t_myproof *g_myproof_pragma;
    extern t_myproof *g_myproof_pass;

#ifdef __cplusplus
}
#endif /* !__cplusplus */

#endif /* !__MYPROOF_H__ */
