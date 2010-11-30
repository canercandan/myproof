#include <stdlib.h>
#include "gcc-plugin.h"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "rtl.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"

int plugin_is_GPL_compatible;
extern struct opt_pass *current_pass;

extern void register_callback (const char*, int, plugin_callback_func, void*);

static bool
our_gate (void)
{
    return true;
}

static unsigned int
our_main (void)
{
    basic_block bb;
    rtx insn;

    FOR_ALL_BB (bb)
    {
        FOR_BB_INSNS (bb, insn)
        {
            debug_rtx (insn);
        }
    }
    return 0;
}

struct opt_pass our_pass = {
    RTL_PASS,
    "RTL add pass",
    our_gate,
    our_main,
    NULL,
    NULL,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};


int
plugin_init (struct plugin_name_args *info, struct plugin_gcc_version *ver)
{
    struct register_pass_info newpass = {&our_pass, "web", 0, PASS_POS_INSERT_AFTER};

    register_callback (info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &newpass);

    return 0;
}

