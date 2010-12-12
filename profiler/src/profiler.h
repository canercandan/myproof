#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATHCG "MyProfCallGrap.dot"
#define PATHINST "profInstsOut.txt"
#define PATHSTATS "profStats.txt"

/*
 * Structure permettant de stocker
 * les données issues de l'analyse statique
 */
typedef struct StaticFunc
{
    char funcName[12];
    int nbloads;
    int nbstores;
} StaticFunc_t;

/*
 * Structure stockant les différentes instances de fonctions
 */
typedef struct InstFunc
{
    char funcName[12];
    int *tabTimeIncl;
    int *tabTimeExcl;
    int nbInsts;
    int nbloads;
    int nbstores;
} InstFunc_t;

/*
 * Noeud de fonction contenant:
 * - les temps start et stop
 * - temps inclusif
 * - temps exclusif
 * - functions enfants
 * - fonction parente
 * - nombre d'enfants
 * - nombre de load
 * - nombre de store
 */
typedef struct funcNode_s
{
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
} funcNode_t;

/*
 * Valeurs de temps d'exécution des load et store
 */
typedef struct ValLoadStore
{
    int valLoad;
    int valStore;
} ValLoadStore_t;

/*
 * Fonction root
 */
extern funcNode_t *g_root;

int initListNodes();
int createNode(char *name, int startTime, int stopTime);
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

#endif // !__PROFILER_H__
