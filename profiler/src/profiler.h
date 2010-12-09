#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATHCG "/home/aurele/MIHP/CPA/Projet/myproof/Partie3/MyProfCallGrap.dot"
#define PATHINST "/home/aurele/MIHP/CPA/Projet/myproof/Partie3/profInstsOut.txt"
#define PATHSTATS "/home/aurele/MIHP/CPA/Projet/myproof/Partie3/profStats.txt"

typedef struct StaticFunc {
    char funcName[12];
    int nbloads;
    int nbstores;
} StaticFunc_t;

typedef struct InstFunc {
    char funcName[12];
    int *tabTimeIncl;
    int *tabTimeExcl;
    int nbInsts;
    int nbloads;
    int nbstores;
} InstFunc_t;

/*
*Structure permettant de stocker les données relatives à une fonction
*/
typedef struct funcNode_s {
    char funcName[12];
    int startTime;
    int stopTime;
    int timeInc;
    int timeExcl;
    struct funcNode_s **funcChilds;
    struct funcNode_s *fNodeParent;
    int numChilds;
    int nloads;
    int nstores;
}funcNode_t;

typedef struct ValLoadStore {
    int valLoad;
    int valStore;
}ValLoadStore_t;

funcNode_t * root;

char* tabFuncName;
int tabFuncNameSize;

float averageTimeLoad;
float averageTimeStore;

int sizeStaticFunc;
StaticFunc_t **tabStaticFunc;

InstFunc_t **tabInstFunc;

FILE *pFile;

char bufferPrint[200];

//FILE *profCG;

funcNode_t **listNodes;
funcNode_t **outFuncNodes;
int sizeListNodes;
int rankList;
int sizeOutFuncNodes;

InstFunc_t **instFuncs;
int sizeInstFuncs;

int addChild(funcNode_t *fNode, funcNode_t **childNode);
int addParent(funcNode_t *fNode, funcNode_t *fParent);
int initListNodes();
int addListNode(funcNode_t * node);
int checkNodeParent(funcNode_t * node);
int computeExclTime(funcNode_t * node, funcNode_t * nodeParent);
int createNode(funcNode_t *fNode, char name[], int startTime, int stopTime);
int instancePresent(int *indexOutFuncNodes);
int manageInstances();
int checkInstances();
int addStaticLoad(int nbloads);
int addStaticStore(int nbstores);
int addStaticFuncName(char * funcName);
int printInstsFuncs();
int initStaticFunc();
int checkStaticFunc();
int correlateDynStatic();
int printFuncStats();
int computeLoadStore();
int printCG(funcNode_t *node);
int parcoursPrefix(funcNode_t *node, int index, char *buffer);
int openFile(FILE *file, char *path);
int closeFile(FILE *file);

#endif // !__PROFILER_H__
