%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
char funcName[10];
int startTime;
int stopTime;
int timeInc;
int timeExcl;
struct funcNode_t **funcChilds;
int numChilds;
}funcNode_t;

typedef funcNode_t tree;

funcNode_t **listNodes;
int sizeListNodes;
int rankList;

int addChild(funcNode_t *parentNode, funcNode_t **childNode)
{
parentNode->numChilds++;
parentNode->funcChilds = (funcNode_t*) realloc(parentNode->funcChilds, parentNode->numChilds * sizeof(funcNode_t)); 
parentNode->funcChilds[parentNode->numChilds] = childNode;
//parentNode->funcChilds = childNode;
return 0;
}

int initListNodes(funcNode_t * tabNodes) 
{
 sizeListNodes = 0;
 rankList=0;
 listNodes = (funcNode_t*)malloc(sizeListNodes*sizeof(funcNode_t)); 

 return 0;
}

int addListNode(funcNode_t * node)
{
 int i;
 sizeListNodes++;
 listNodes = (funcNode_t *)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
 listNodes[rankList] = node;
 rankList++;
  
 return 0;
}

int checkNodeParents(funcNode_t * node)
{
 int i;

 if(rankList>0) {
  for(i=0;i<=rankList;i++) {
   if((node->startTime>(listNodes[rankList]->startTime)) && (node->stopTime<(listNodes[rankList]->stopTime))) {
    addChild(listNodes[rankList], node);
   }
  }
 }

 return 0;
}

int computeExclTime(funcNode_t * node)
{
 int i;
 int tempTime = 0;
 funcNode_t *tempNode;
 
 for(i=0;i<(node->numChilds);i++) {
  tempNode = node->funcChilds[i];
  tempTime +=tempNode->timeInc;
 }
 
 node->timeExcl = node->timeInc-tempTime;

 return 1;
}

int createNode(funcNode_t *fNode, char name[], int startTime, int stopTime)
{
 strcpy(fNode->funcName, name);
 fNode->startTime = startTime;
 fNode->stopTime = stopTime;
 fNode->funcChilds = NULL;

 fNode->funcChilds = malloc(0);
 fNode->numChilds = 0;

 fNode->timeInc = fNode->stopTime-fNode->startTime;

 checkNodeParents(fNode);
 addListNode(fNode);
 
 computeExclTime(fNode);

 return 0;
}

%}

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

%token CALL
%token FUNCTION
%token CYCLE
%token ENTER
%token EXIT
%token TIME
%token NAME

%%

exp: MNEMONIC OPERAND COMMA OPERAND {return 1;}
    | LABEL {return 1;}
    | LOAD {return 1;}
    | CALL {return 1;}
    | CALL FUNCTION NAME ENTER CYCLE TIME EXIT CYCLE TIME {printf("function %s, enter cycle %d, exit cycle %d\n", $3, $6, $9);}
    | exp CALL FUNCTION NAME ENTER CYCLE TIME EXIT CYCLE TIME  {printf("function %s, enter cycle %d, exit cycle %d\n", $2, $7, $9);}
    ;

OPERAND: ADDRESS
       | REG
       ;

MNEMONIC: LOAD
        ;

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

int yyerror(const char *str) {
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

int main(void)
{
//FuncNode *mainNode;

printf("debut parsing\n");


yyparse();
}
