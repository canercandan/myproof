
#include <stdlib.h>
#include "gcc-plugin.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-pass.h"
//#include "expr.h"
#include "cfgloop.h"
//#include "optabs.h"

#define LEFT 0
#define RIGHT 1

int plugin_is_GPL_compatible;
extern struct opt_pass *current_pass;

int nbloads;
int nbstores;

static bool
our_gate (void)
{
    return true;
}


/*
 * mihp_get_type_size
 */
int myprof_get_type_size ( tree );

int
myprof_get_type_size ( tree t )
{
  int ret=0;

  if ( TYPE_SIZE_UNIT(t) != NULL )
    ret = TREE_INT_CST_LOW ( TYPE_SIZE_UNIT(t) );

  return ret;
}


/*
 * MIHP pass
 */
static void
myprof_read_type ( tree t )
{
  enum tree_code tc;

  tc = TREE_CODE ( t );

  switch ( tc )
    {
    case INTEGER_TYPE:
      //printf("L'operation manipule des entiers\n");
    break;
    case BOOLEAN_TYPE:
    case REAL_TYPE:
      /* scalar */
      //printf("L'operation manipule des réels\n");
      break;
    case ARRAY_TYPE:
     break;
    case INDIRECT_REF:
     printf("case INDIRECT_REF detected\n");
     break;
    default:
      fprintf ( stderr, "mihp: mihp_read_type(): %s is not handled\n", tree_code_name[tc] );
      gcc_unreachable ( );
    }
}

static void
myprof_read_data ( tree t )
{

/* liste* l;*/
/* */
/* if(l->taille==0) {*/
/*  l->t = t;*/
/*  l->taille++;*/
/* }*/
/* */
/* if(existTree(l, t) == 0) {*/
/*  l->next->t  = t;*/
/* }*/
/*  */
 //strcpy(l->chaine, IDENTIFIER_POINTER(DECL_NAME(t)));
 
  
  
/*  if ( DECL_NAME(t) )*/
/*    printf ( "       data \'%s\'", IDENTIFIER_POINTER(DECL_NAME(t)) );*/
/*  else*/
/*    printf ( "       data \'%c_%u\'", (TREE_CODE(t)==CONST_DECL)?'C':'D', DECL_UID(t) );*/
  
/*  printf("\n");*/

  myprof_read_type ( TREE_TYPE(t) );
}

static void
myprof_read_operand ( tree t, int position )
{
  enum tree_code tc;

  tc = TREE_CODE ( t );

 //printf("On passe par la\n");
 //printf("taille variable: %d\n", mihp_get_type_size(t));

  switch ( tc )
    {
    case VAR_DECL:
     //printf("variable detectee\n");
     break;
    case PARM_DECL:
    case CONST_DECL:
      myprof_read_data ( t );
      break;

    case ARRAY_REF:
/*      fprintf(stdout, "array operand\n");*/
      myprof_read_operand ( TREE_OPERAND(t,0), 2); /* array base */
      myprof_read_operand ( TREE_OPERAND(t,1), 2); /* array index */
      
      if(position == LEFT) {
       nbstores++;
/*       printf("1 autre store\n");*/
/*       fprintf(stdout, "store\n");*/
      }
      else if(position == RIGHT) {
 	 nbloads++;
/*         printf("1 autre load\n");*/
/*       fprintf(stdout, "load\n");*/
      }
       
      break;

    case ADDR_EXPR:
     break;
    case INDIRECT_REF:
      /* pointer & dereferencing */
      break;

    case INTEGER_CST:
/*      printf("L'operation est de type entier\n");*/
    case REAL_CST:
/*      printf("L'operation est de type reel\n");*/
    case STRING_CST:
      /* integer/real/string constant */
      break;

    case SSA_NAME:
      myprof_read_data ( SSA_NAME_VAR(t) );
      break;

    default:
      fprintf ( stderr, "mihp: mihp_read_operand(): unhandled \'%s\'\n", tree_code_name[tc] );
      gcc_unreachable ( );
    }
}

static void
myprof_read_stmt ( gimple g, int *nbop)
{
  unsigned int i;
  enum gimple_code gc;
  
/*  nbloads = 0;*/
/*  nbstores = 0;*/
  
/*  fprintf(stderr, "nbop dans mihp_read_stmt: %d\n", *nbop);*/

  gc = gimple_code ( g );

  switch ( gc )
    {
    case GIMPLE_ASSIGN:
     (*nbop)++;
     //fprintf(stderr, "dans mihp_read_stmt, nombre d'operations: %d\n", *nbop);
     //Premier opérande à droite de l'égalité
      myprof_read_operand ( gimple_op(g,1), RIGHT ); /* op1 */

      if ( gimple_num_ops(g) > 2 )  {/* op2 */
/*          printf("Plus de 2 operandes\n");*/
/*          printf("Type d'operation: %d\n", gimple_assign_rhs_code(g));*/
/*          printf("Type d'operation: %s\n", tree_code_name[gimple_assign_rhs_code(g)]);*/
 	  //nb2opSuperior++;
          myprof_read_operand ( gimple_op(g,2), RIGHT );
          
          //debug_tree(gimple_op(gsi_stmt(gsi),1));
          tree type1 = TREE_TYPE(gimple_op(g,1));
          tree type2 = TREE_TYPE(gimple_op(g,2));
          
/*	  debug_tree(type1);*/
/*	  debug_tree(type2);*/
	  
	  int tc = TREE_CODE(type1);
	  int tc2 = TREE_CODE(type2);
	  
	  if((tc == REAL_TYPE) || (tc2 == REAL_TYPE)) {
/*	   fprintf(stderr, "l'operation est de type reel\n");*/
	  }
	  else {
/*	   fprintf(stderr, "l'operation est de type entier\n");*/
	  }
	  
/*	  if(tc == REAL_TYPE) {*/
/*	   fprintf(stderr, "type Réel\n");*/
/*	  } */
/*	  else if(tc == INTEGER_TYPE) {*/
/*	   fprintf(stderr, "type Entier\n");*/
/*	  }*/
 
      }

      myprof_read_operand ( gimple_op(g,0), LEFT ); /* op def */
      break;

    case GIMPLE_CALL:
      for ( i=0; i<gimple_call_num_args(g); ++i )
        myprof_read_operand ( gimple_call_arg(g,i), 2 );

      if ( gimple_call_lhs(g) != NULL_TREE )
        myprof_read_operand ( gimple_call_lhs(g), 2 );
      break;

    case GIMPLE_COND:
      myprof_read_operand ( gimple_cond_lhs(g), 2 ); /* op1 */
      myprof_read_operand ( gimple_cond_rhs(g), 2 ); /* op2 */
      break;

    case GIMPLE_RETURN:
      if ( gimple_return_retval(g) != NULL_TREE )
        myprof_read_operand ( gimple_return_retval(g), 2 );
      break;

    default:
      fprintf ( stderr, "mihp: mihp_read_stmt(): unhandled \'%s\'\n", gimple_code_name[gc] );
      gcc_unreachable ( );
    }
    

    
}

static void
myprof_read_loop ( struct loop *l )
{
  struct loop *inner;

  fprintf ( stderr, " # loop %d", l->num );

  if ( double_int_zero_p(l->nb_iterations_estimate) ) /* double_int type defined in double-int.h */
    fprintf ( stderr, " has unknown number of iterations" );
  else
    fprintf ( stderr, " has %lu iterations", l->nb_iterations_estimate.low );

/*  fprintf ( stderr, "\n" );*/

  for ( inner=l->inner; inner!=NULL; inner=inner->next )
    myprof_read_loop ( inner );
}

static unsigned int
myprof_pass ( void )
{
  basic_block bb;
  gimple_stmt_iterator gsi;

  int nbop = 0;

  //printf("Debut de la passe\n");

  fprintf ( stderr, "* MIHP on %s()\n", IDENTIFIER_POINTER(DECL_NAME(cfun->decl)) );

  FOR_EACH_BB ( bb )
    {
       fprintf ( stdout, "  ** BB %d\n", bb->index ); 

       //*nb2opSuperior=0;
/*       nbop = 0;*/

     nbloads = 0;
     nbstores = 0;

      for ( gsi=gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi) )
      {
/*          print_gimple_stmt ( stdout, gsi_stmt(gsi), 0, 0 );*/
	        
          myprof_read_stmt ( gsi_stmt(gsi), &nbop);
      }
        
                  
     if(nbloads > 0) 
      fprintf(stdout, "%d load\n", nbloads);
     
     if(nbstores > 0)
      fprintf(stdout, "%d store\n", nbstores);
        
/*        fprintf(stdout, "nombre d'operations: %d\n", nbop);*/

      //printf("Nombre d'operations superieures à 2: %d\n", nb2opSuperior);
    }

  /* recursively read looops */
  if ( cfun->x_current_loops != NULL )
    myprof_read_loop ( cfun->x_current_loops->tree_root );

  return 0;
}

static bool
mihp_gate ( void )
{
/*  return flag_mihp != 0;*/
return true;
}

static unsigned int
our_main (void)
{
/*
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
    */
    
    return myprof_pass();
}



struct opt_pass our_pass = {
    GIMPLE_PASS,
    "MYPROF add pass",
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
    struct register_pass_info newpass = {&our_pass, "mudflap2", 0, PASS_POS_INSERT_AFTER};

    register_callback (info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &newpass);

    return 0;
}

