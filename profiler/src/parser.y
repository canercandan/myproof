%{
#include "BuildProfile.h"
%}

%union {
    int value;
    char *str;
};

%token LOAD
%token STORE
%token REG
%token ADD
%token SUB
%token COMMA
%token LABEL
%token CMP
%token GOTO
%token ADDRESS
%token FACTOR
%token MUL
%token RETLINE

%token CALL
%token FUNCTION
%token ENTERCYCLE
%token EXITCYCLE
%token <value>NUMERIC
%token <str>NAME

%type <str> BODYSTATIC


%%

//expstatic: | expstatic NBLOADS
//         ;
//
//FUNCNAME: FUNCTION NAME RETLINE
//        ;
//
//BODYSTATIC: | BODYSTATIC NBLOADS
//          | BODYSTATIC NBSTORES
//          | NBLOADS
//          | NBSTORES
//          ;

//NBLOADS: FACTOR LOAD RETLINE
//       | FACTOR MUL FACTOR LOAD RETLINE
//       ;
//
//NBSTORES: FACTOR STORE RETLINE
//        | FACTOR MUL FACTOR STORE RETLINE
//        ;

exp: FUNCTION NAME RETLINE
{
    printf("static file recognized\n");
    printf("function name: %s\n", $2);
    char *nameFunc = strtok($2, " ");
    addStaticFuncName(nameFunc);
}
| exp FUNCTION NAME RETLINE
{
    printf("static file recognized\n");
    printf("function name: %s\n", $3);
    char *nameFunc = strtok($3, " ");
    addStaticFuncName(nameFunc);
}
| exp CALL FUNCTION NAME ENTERCYCLE NUMERIC EXITCYCLE NUMERIC RETLINE
{
    printf("\n$4: %s \n", $4);
    char * nameFunc = strtok($4, " ");
    printf("name of function: %s\n", nameFunc);
    printf("enter cycle time: %d ", $6);
    printf("exit cycle: %d ", $8);
    funcNode_t *funcNode2;
    const char *name = "tata";
    int start = $6;
    int stop = $8;
    createNode(funcNode2, nameFunc, start, stop);
}
| CALL FUNCTION NAME ENTERCYCLE NUMERIC EXITCYCLE NUMERIC RETLINE
{
    printf("\n$3: %s \n", $3);
    char * tempStr = strtok($3, " ");
    printf("tempStr: %s\n", tempStr);
    char * nameFunc = strtok($3, " ");
    strcat(nameFunc, "\0");
    printf("Longueur nom: %d\n", strlen(nameFunc));
    printf("name of function: %s\n", nameFunc);
    printf("enter cycle time: %d ", $5);
    printf("exit cycle: %d ", $7);
    funcNode_t *funcNode1;
    const char *name = "toto";
    int start = $5;
    int stop = $7;
    createNode(funcNode1, nameFunc, start, stop);
}
| exp NUMERIC LOAD RETLINE
{
    printf("loads recognized\n");
    int nbLoads = $2;
    printf("nbLoads: %d\n", nbLoads);
    addStaticLoad(nbLoads);
}
| exp NUMERIC MUL NUMERIC LOAD RETLINE
{
    printf("loads recognized\n");
    int nbLoads = $2*$4;
    printf("nbLoads: %d\n", nbLoads);
    addStaticLoad(nbLoads);
}
| exp NUMERIC STORE RETLINE
{
    printf("stores recognized\n");
    int nbStores = $2;
    printf("nbStores: %d\n", $2);
    addStaticStore(nbStores);
}
| exp NUMERIC MUL NUMERIC STORE RETLINE
{
    printf("stores recognized\n");
    int nbStores = $2*$4;
    printf("nbStores: %d\n", nbStores);
    addStaticStore(nbStores);
}
;

//FUNCNAME: FUNCTION NAME RETLINE
//        ;

BODYSTATIC: | BODYSTATIC NBLOADS RETLINE
| BODYSTATIC NBSTORES RETLINE
| NBLOADS RETLINE
| NBSTORES RETLINE
;

NBLOADS: NUMERIC LOAD
| NUMERIC MUL NUMERIC LOAD
;

NBSTORES: NUMERIC STORE
| NUMERIC MUL NUMERIC STORE
;

//OPERAND: ADDRESS
//       | REG
//       ;

//MNEMONIC: LOAD
//        ;

%%

/*
%%
STATEMENT: CALL FUNCTION ENTER CYCLE EXIT CYCLE {}
         ;
%%
//statement: CALL FUNCTION STARTTIME STOPTIME { }
//         ;
//statement: CALL FUNCTION
//         ;

//STARTTIME: ENTER CYCLE TIME {starttime = TIME;}
//         ;

//STOPTIME: EXIT CYCLE TIME {stoptime = TIME;}
//	;
*/


/*
int addFunc()
{
 FuncNode* fNode;
 strcpy(fNode->name, NAME);
 fNode->start_time = starttime;
 fNode->stop_time = stoptime;
 return 0;
}
*/

int yyerror(const char *str)
{
    printf("error detected: ");
    printf("%s\n", str);
}

/*
  int yywrap()
  {
  // fonction appelé par defaut a la fin de l'analyse syntaxique
  printf ("Fin de l'analyse syntaxique \n");
  return 1;
  }
*/

///////////////////////////////////////////
//                                       //
// Point d'entrée de l'analyseur         //
//                                       //
///////////////////////////////////////////

int main(void)
{
    //FuncNode *mainNode;

    printf("Début profiling\n");

    //funcNode_t * root;
    initListNodes();
    initStaticFunc();

    yyparse();

    manageInstances();

    checkInstances();
    checkStaticFunc();
    correlateDynStatic();
    computeLoadStore();
    printFuncStats();
    printInstsFuncs();
    printf("root childs: %d\n", root->numChilds);

    //FILE *profCG;
    //openFile(profCG, "/home/aurele/MIHP/CPA/Projet/myproof/Partie3/MyProfCallGrap.dot");
    //printf("prefix\n");
    //printPrefix(root->funcChilds[1], 1);
    printCG(root->funcChilds[1]);
    //closeFile(profCG);

    printf("Fin du profiling\n");

    return 0;
}
