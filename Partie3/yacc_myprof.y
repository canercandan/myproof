%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int time1;

char* tabFuncName;
int tabFuncNameSize;

typedef struct InstFunc {
 char funcName[10];
 int *tabTimeIncl;
 int *tabTimeExcl;
 int nbInsts;
 int nbloads;
 int nbstores;
} InstFunc_t;

InstFunc_t **tabInstFunc;

FILE *pFile;

/*
*Structure permettant de stocker les données relatives à une fonction
*/
typedef struct node {
char funcName[10];
int startTime;
int stopTime;
int timeInc;
int timeExcl;
struct funcNode_t **funcChilds;
struct funcNode_t *fNodeParent;
int numChilds;
//int nbloads;
}funcNode_t;

//struct funcNode_t * root;

funcNode_t **listNodes;
funcNode_t **outFuncNodes;
int sizeListNodes;
int rankList;
int sizeOutFuncNodes;

InstFunc_t **instFuncs;
int sizeInstFuncs;


/*
* addChild function
*/
int addChild(funcNode_t *fNode, funcNode_t **childNode)
{
printf("****Begin addChild****\n");

fNode->numChilds++;
fNode->funcChilds = (funcNode_t*) realloc(fNode->funcChilds, fNode->numChilds * sizeof(funcNode_t)); 
fNode->funcChilds[fNode->numChilds] = childNode;
//parentNode->funcChilds = childNode;

printf("****End addChild****\n");

return 0;
}

/*
* addParent function
*/
int addParent(funcNode_t *fNode, funcNode_t *fParent)
{
printf("****Begin addParent****\n");

 fNode->fNodeParent = fParent;

printf("****End addParent****\n"); 
 return 0;
}


/*
*initListNodes function
*/
int initListNodes(funcNode_t * root) 
{
printf("****Begin initListNodes****\n");
 
 //Initialize output stream
 pFile = fopen("/home/aurele/MIHP/CPA/Projet/myproof/Partie3/output.txt","w");
 
 sizeListNodes = 1;
 rankList=0;
 listNodes = (funcNode_t*)malloc(sizeListNodes*sizeof(funcNode_t)); 
 
 root = malloc(sizeof(funcNode_t));
 strcpy(root->funcName, "root");
 printf("strcpy ok\n");
 listNodes[rankList] = root;
 
 //Init tabFuncName
 tabFuncNameSize = 0;
 tabFuncName = (char*)malloc(tabFuncNameSize*sizeof(char));
 
 //Init InstFunc 
 sizeInstFuncs = 0;
 sizeOutFuncNodes = 0;
 tabInstFunc = (InstFunc_t *)malloc(sizeInstFuncs*sizeof(InstFunc_t));
  outFuncNodes = (funcNode_t *)malloc(sizeOutFuncNodes*sizeof(funcNode_t));
 
 printf("****End initListNodes****\n");

 return 0;
}

/*
* addListNode function
*/
int addListNode(funcNode_t * node)
{
 printf("****Begin addListNode****\n");
 printf("rankList: %d\n", rankList);

 if(node != NULL) {
  if(rankList > 1) {
   if(node->startTime > listNodes[rankList-1]->startTime) {
    int i;
    sizeListNodes++;
    listNodes = (funcNode_t *)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
    listNodes[rankList] = node;
    rankList++;
   }
   else {
    fprintf(stderr, "function must start after the previous one\n");
    return -1;
   }
  }
  else {
    int i;
    sizeListNodes++;
    listNodes = (funcNode_t *)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
    listNodes[rankList] = node;
    rankList++;
  }
 }
 
 printf("node startTime at rankList %d: %d\n", rankList, listNodes[rankList-1]->startTime);
 printf("rankList: %d\n", rankList);
 
 printf("****End addListNode*****\n");
  
 return 0;
}

/*
* checkNodeParent function
*/
int checkNodeParent(funcNode_t * node)
{
printf("****Begin CheckNodeParent****\n");
 int i;
 
 if(node == NULL) {
  printf("node NULL\n");
 }
 
 funcNode_t **tempParentTab;
 int sizeParents = 0;
 tempParentTab = (funcNode_t*)malloc(0*sizeof(funcNode_t));

 if(rankList>0) {
  printf("rankList: %d\n", rankList);
  for(i=0;i<rankList;i++) {
//   printf("node startTime: %d\n", node->startTime);
//   printf("node stopTime: %d\n", node->stopTime);
   printf("node startTime at rankList %d: %d\n", i, listNodes[i]->startTime);
   if((node->startTime > (listNodes[i]->startTime)) && (node->stopTime<(listNodes[i]->stopTime))) {
    sizeParents++;
    tempParentTab = (funcNode_t*)realloc(tempParentTab, sizeParents*sizeof(funcNode_t));
    tempParentTab[sizeParents-1] = listNodes[i];
   }
  }
    if(sizeParents > 0) {
     printf("sizeParents: %d\n", sizeParents);
     printf("tempParentTab name: %s\n", tempParentTab[sizeParents-1]->funcName);
     //printf("addChild at rankList %d\n", rankList);
     //addChild(listNodes[rankList-1], node);
     addChild(tempParentTab[sizeParents-1], node);
     addParent(node, tempParentTab[sizeParents-1]);
    }  
  
 }
 
 printf("****End CheckNodeParent****\n");

 return 0;
}

/*
* computeExclTime function
*/
int computeExclTime(funcNode_t * node, funcNode_t * nodeParent)
{
printf("****Begin computeExclTime****\n");

 int i;
 int tempTime = 0;
// funcNode_t *tempNode;
 
// for(i=0;i<(node->numChilds);i++) {
//  tempNode = node->funcChilds[i];
//  tempTime +=tempNode->timeInc;
// }
 
 nodeParent->timeExcl = nodeParent->timeExcl - node->timeInc;
 
// printf("tempTime: %d\n", tempTime);
 
 //node->timeExcl = node->timeInc-tempTime;
 
 fprintf(pFile,"nodeParent %s exclusive time: %d\n",nodeParent->funcName, nodeParent->timeExcl);
 
 printf("****End computeExclTime****\n");

 return 1;
}



/*
* createNode() function
*/
int createNode(funcNode_t *fNode, char name[], int startTime, int stopTime)
{
 printf("****begin createNode****\n");
 
 int i;

 if(startTime > stopTime) {
  printf("startTime > stopTime! \n");
  return -1; 
 } 
 
 printf("name : %s\n", name);
 
 fNode = malloc(sizeof(funcNode_t));
 strcpy(fNode->funcName, name);
 printf("strcpy ok\n");
 fNode->startTime = startTime;
 fNode->stopTime = stopTime;
 fNode->funcChilds = NULL;

 fNode->funcChilds = (funcNode_t *)malloc(0*sizeof(funcNode_t));
 fNode->numChilds = 0;

 fNode->timeInc = fNode->stopTime-fNode->startTime;
 fNode->timeExcl = fNode->timeInc;
 printf("fNode name: %s\n", fNode->funcName);
// printf("fNode startTime: %d\n", fNode->startTime);
// printf("fNode stopTime: %d\n", fNode->stopTime);
 fprintf(pFile,"function %s inclusive time: %d\n",fNode->funcName, fNode->timeInc);

 checkNodeParent(fNode);
 int returnFct = addListNode(fNode);
 
 if(returnFct == -1) {
  return -1;
 }
 
 funcNode_t *fTempNodeParent = fNode->fNodeParent;
 
 printf("fNode %s parent: %s\n", fNode->funcName, fTempNodeParent->funcName);
 
 if(fNode->fNodeParent != NULL)
  computeExclTime(fNode, fNode->fNodeParent);
  
 printf("fNode numChilds: %d\n", fNode->numChilds);
 
 sizeOutFuncNodes++;
 outFuncNodes = (funcNode_t*)realloc(outFuncNodes, sizeOutFuncNodes*sizeof(funcNode_t));
 outFuncNodes[sizeOutFuncNodes-1] = fNode;
  
  printf("****end createNode****\n");

 return 0;
}

/*
* Check if an instance of function is already present
*/ 
int instancePresent(int *indexOutFuncNodes)
{
   int j;
   printf("outFuncNodes %d name: %s\n",*indexOutFuncNodes, outFuncNodes[*indexOutFuncNodes]->funcName);
   
   for(j=0;j<sizeInstFuncs;j++) {
   
   //Si la fonction existe déjà dans le tableau d'instances, alors on rajoute l'instance 
   //printf("tab Instance function name: %s\n", tabInstFunc[j]->funcName);
   
   if(strcmp(outFuncNodes[*indexOutFuncNodes]->funcName, tabInstFunc[j]->funcName) == 0) {

    printf("Add an instance of function\n");
    
    printf("function name: %s\n", tabInstFunc[j]->funcName);
    //nbInsts++;
    tabInstFunc[j]->nbInsts++;
    tabInstFunc[j]->tabTimeExcl = (int *)realloc(tabInstFunc[j]->tabTimeExcl, (tabInstFunc[j]->nbInsts)*sizeof(int));
    tabInstFunc[j]->tabTimeIncl = (int *)realloc(tabInstFunc[j]->tabTimeIncl, (tabInstFunc[j]->nbInsts)*sizeof(int));
        
    printf("function node exclusive time: %d\n", outFuncNodes[*indexOutFuncNodes]->timeExcl);
    printf("function node inclusive time: %d\n", outFuncNodes[*indexOutFuncNodes]->timeInc);
        
    tabInstFunc[j]->tabTimeExcl[tabInstFunc[j]->nbInsts] = outFuncNodes[*indexOutFuncNodes]->timeExcl;
    tabInstFunc[j]->tabTimeIncl[tabInstFunc[j]->nbInsts] = outFuncNodes[*indexOutFuncNodes]->timeInc;
    
     j=sizeInstFuncs;
     
     return 1;
   }
  }
 
 return 0;
}


int manageInstances()
{

 //Manage instances
 printf("\n***Begin Instances management***\n");
 
 int i, j;
 
  for(i=0;i<sizeOutFuncNodes;i++) {
   printf("outFuncNodes %d name: %s\n",i, outFuncNodes[i]->funcName);
   printf("outFuncNodes %d inclusive time: %d\n", i, outFuncNodes[i]->timeInc);
   printf("outFuncNodes %d exclusive time: %d\n", i, outFuncNodes[i]->timeExcl);
   printf("\n");
  } 
  
 for(i=0;i<sizeOutFuncNodes;i++) {
 
 printf("outFuncNodes %d nameFunc: %s\n",i, outFuncNodes[i]->funcName);
 
 if(sizeInstFuncs<1) {
  //tabFuncName = (char *)realloc(tabFuncName, tabFuncNameSize*sizeof(char));
  //strcpy(tabFuncName[tabFuncNameSize]->funcName, fNode->funcName); 
  //tabFuncNameSize++;  
  printf("Create the first function in tabInstFunc\n");
  
  sizeInstFuncs++; 
  tabInstFunc = (InstFunc_t *)realloc(tabInstFunc, sizeInstFuncs*sizeof(InstFunc_t));
  printf("tabInstFunc realloc ok\n");
  
  tabInstFunc[sizeInstFuncs-1] = malloc(sizeof(InstFunc_t));
  
  strcpy(tabInstFunc[sizeInstFuncs-1]->funcName, outFuncNodes[i]->funcName);
  printf("name copy ok\n");
  
  tabInstFunc[sizeInstFuncs-1]->nbInsts = 1;
  tabInstFunc[sizeInstFuncs-1]->tabTimeExcl = (int*)realloc(tabInstFunc[sizeInstFuncs-1]->tabTimeExcl, tabInstFunc[sizeInstFuncs-1]->nbInsts*sizeof(int));
  tabInstFunc[sizeInstFuncs-1]->tabTimeIncl = (int*)realloc(tabInstFunc[sizeInstFuncs-1]->tabTimeIncl, tabInstFunc[sizeInstFuncs-1]->nbInsts*sizeof(int));
  
  tabInstFunc[sizeInstFuncs-1]->tabTimeExcl[tabInstFunc[sizeInstFuncs-1]->nbInsts] = outFuncNodes[i]->timeExcl;
  tabInstFunc[sizeInstFuncs-1]->tabTimeIncl[tabInstFunc[sizeInstFuncs-1]->nbInsts] = outFuncNodes[i]->timeInc;
  
  //nbInsts++;
  
 }
 else {
 
   if(instancePresent(&i) == 0) {
   
    //On créé la fonction dans le tableau d'instances
    printf("Create a new function in tabInstFunc\n");
    sizeInstFuncs++;
    printf("size of InstFuncs tab: %d\n", sizeInstFuncs);
    
    tabInstFunc = (InstFunc_t*)realloc(tabInstFunc, sizeInstFuncs*sizeof(InstFunc_t));
    printf("realloc ok\n");
    tabInstFunc[sizeInstFuncs-1] = malloc(sizeof(InstFunc_t));
      
    strcpy(tabInstFunc[sizeInstFuncs-1]->funcName, outFuncNodes[i]->funcName);
    printf("copy name %s\n", outFuncNodes[i]->funcName);
    tabInstFunc[sizeInstFuncs-1]->nbInsts = 1;
    tabInstFunc[sizeInstFuncs-1]->tabTimeExcl = (int*)realloc(tabInstFunc[sizeInstFuncs-1]->tabTimeExcl, (tabInstFunc[sizeInstFuncs-1]->nbInsts)*sizeof(int));
    tabInstFunc[sizeInstFuncs-1]->tabTimeIncl = (int*)realloc(tabInstFunc[sizeInstFuncs-1]->tabTimeIncl, (tabInstFunc[sizeInstFuncs-1]->nbInsts)*sizeof(int));
    
    printf("outFuncNodes %d inclusive time: %d\n", i, outFuncNodes[i]->timeInc);
    printf("outFuncNodes %d exclusive time: %d\n", i, outFuncNodes[i]->timeExcl);
    
   tabInstFunc[sizeInstFuncs-1]->tabTimeExcl[tabInstFunc[sizeInstFuncs-1]->nbInsts] = outFuncNodes[i]->timeExcl;
   tabInstFunc[sizeInstFuncs-1]->tabTimeIncl[tabInstFunc[sizeInstFuncs-1]->nbInsts] = outFuncNodes[i]->timeInc;
   
    printf("instFuncs %d inclusive time: %d\n", i, tabInstFunc[sizeInstFuncs-1]->tabTimeIncl[tabInstFunc[sizeInstFuncs-1]->nbInsts]);
    printf("instFuncs %d exclusive time: %d\n", i, tabInstFunc[sizeInstFuncs-1]->tabTimeExcl[tabInstFunc[sizeInstFuncs-1]->nbInsts]);
  
   j=sizeInstFuncs;
  }	
  
 }

 }
 
 for(i=0;i<sizeInstFuncs;i++) {
    printf("instFuncs %d function name %s inclusive time: %d\n", i, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeIncl[1]);
    printf("instFuncs %d function name %s exclusive time: %d\n", i, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeExcl[1]);
 }

  printf("\n***End Instances management***\n");
  
return 0;
}


/*
* checkInstances function
*/
int checkInstances()
{
  printf("****Begin checkInstances****\n");
  
  int i,j; 
  
  printf("Taille InstFuncs: %d\n", sizeInstFuncs);
 
  for(i=0;i<sizeInstFuncs;i++) {
   printf("Function names: %s\n", tabInstFunc[i]->funcName);
   
//   if(tabInstFunc[i]->nbInsts > 1) {
//    printf("%d instances of function %s\n", tabInstFunc[i]->nbInsts, tabInstFunc[i]->funcName);
    
    for(j=1;j<=tabInstFunc[i]->nbInsts;j++) {
     printf("instance %d of function %s 's exclusive time: %d\n", j, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeExcl[j]);
     printf("instance %d of function %s 's inclusive time: %d\n", j, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeIncl[j]);
    }
//   }
  }
 
  printf("****End checkInstances****\n");
  
return 0;
}

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
%token <value>TIME
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

exp: | exp FUNCTION NAME BODYSTATIC
     {
      printf("static file recognized\n");
     }
     | exp CALL FUNCTION NAME ENTERCYCLE TIME EXITCYCLE TIME RETLINE
    {
     printf("\n$4: %s \n", $4); 
     char * nameFunc = strtok($4, " ");
     printf("name of function: %s\n", nameFunc); 
     printf("enter cycle time: %d ", $6); 
     printf("exit cycle: %d ", $8);
     funcNode_t *funcNode2;
     char name[4] = "tata";
     int start = $6;
     int stop = $8;
     createNode(funcNode2, nameFunc, start, stop);
    }
    | CALL FUNCTION NAME ENTERCYCLE TIME EXITCYCLE TIME RETLINE
    {
     printf("\n$3: %s \n", $3);
     char * nameFunc = strtok($3, " ");
     printf("name of function: %s\n", nameFunc); 
     printf("enter cycle time: %d ", $5); 
     printf("exit cycle: %d ", $7);
     funcNode_t *funcNode1;
     char name[4] = "toto";
     int start = $5;
     int stop = $7;
     createNode(funcNode1, nameFunc, start, stop);
    }
    | FUNCTION NAME BODYSTATIC
    {
     printf("static analysis\n");
     printf("function: %s\n", $2);
     printf("name: %s\n", $2);
     printf("body: %s\n", $3);
    }
    ;
    
//FUNCNAME: FUNCTION NAME RETLINE
//        ;
        
BODYSTATIC: | BODYSTATIC NBLOADS RETLINE
          | BODYSTATIC NBSTORES RETLINE
          | NBLOADS RETLINE
          | NBSTORES RETLINE
          ;

NBLOADS: FACTOR LOAD RETLINE
       | FACTOR MUL FACTOR LOAD RETLINE
       ;
       
NBSTORES: FACTOR STORE RETLINE
        | FACTOR MUL FACTOR STORE RETLINE
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

printf("Début profiling\n");

funcNode_t * root;
initListNodes(root);

yyparse();

manageInstances();

checkInstances();

printf("Fin du profiling\n");

return 0;
}
