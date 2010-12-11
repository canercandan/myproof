%{
#include "profiler.h"
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
    int start = $6;
    int stop = $8;
    createNode(nameFunc, start, stop);
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
    int start = $5;
    int stop = $7;
    createNode(nameFunc, start, stop);
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

%%

int yyerror(const char *str)
{
    printf("error detected: ");
    printf("%s\n", str);
    return 1;
}

funcNode_t *g_root;

///////////////////////////////////////////
//                                       //
// Point d'entrée de l'analyseur         //
//                                       //
///////////////////////////////////////////

int main(void)
{
    printf("Début profiling\n");

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

    printf("root childs: %d\n", g_root->numChilds);

    if (g_root->funcChilds)
	{
	    printCG(g_root->funcChilds[1]);
	}

    printf("Fin du profiling\n");

    return 0;
}
