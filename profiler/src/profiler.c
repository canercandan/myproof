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

#include <stdio.h>
#include "profiler.h"

static char *tabFuncName;
static int tabFuncNameSize;

static float averageTimeLoad;
static float averageTimeStore;

static int sizeStaticFunc;
static StaticFunc_t **tabStaticFunc;

static InstFunc_t **tabInstFunc;

static FILE *pFile;

static char bufferPrint[200];

static funcNode_t **listNodes;
static funcNode_t **outFuncNodes;
static int sizeListNodes;
static int rankList;
static int sizeOutFuncNodes;

static int sizeInstFuncs;

/*
 * Ajout d'un noeud enfant
 */
static int addChild(funcNode_t *fNode, funcNode_t **childNode)
{

    fNode->numChilds++;
    fNode->funcChilds = (void*) realloc(fNode->funcChilds, fNode->numChilds * sizeof(funcNode_t));
    fNode->funcChilds[fNode->numChilds] = (void*) childNode;

    return 0;
}

/*
 * Ajout d'un noeud parent
 */
static int addParent(funcNode_t *fNode, funcNode_t *fParent)
{
    fNode->fNodeParent = fParent;

    return 0;
}

/*
 * Ajout d'un noeud dans la liste
 */
static int addListNode(funcNode_t * node)
{

    if(node != NULL)
	{
	    if(rankList >= 1)
		{
		    //On vérifie que les temps d'exécution analysées sont cohérentes
		    if ( ( (node->startTime > listNodes[rankList-1]->startTime) && (node->stopTime < listNodes[rankList-1]->stopTime) )
			 || ( (node->startTime > listNodes[rankList-1]->stopTime) && (node->stopTime > listNodes[rankList-1]->stopTime) ) )
			{
			    sizeListNodes++;
			    listNodes = (void*)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
			    listNodes[rankList] = node;
			    rankList++;
			}
		    else
			{
			    fprintf(stderr, "Measure error\n");
			    return -1;
			}
		}
	    else
		{
		    sizeListNodes++;
		    listNodes = (void*)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
		    listNodes[rankList] = node;
		    addParent(node,g_root);
		    rankList++;
		}
	}

    return 0;
}

/*
 * Vérifier si le noeud précédent est un parent
 */
static int checkNodeParent(funcNode_t *node)
{
    int i;

    if(node == NULL)
	{
	    //printf("node NULL\n");
	}

    funcNode_t **tempParentTab;
    int sizeParents = 0;

    tempParentTab = (void*)malloc(0*sizeof(funcNode_t));

    if(rankList == 0)
	{
	    addChild(g_root, (void*)node);
	    addParent(node, g_root);
	}

    if(rankList > 0)
	{

	    for(i = 0; i < rankList; i++)
		{
		    //Vérification de l'imbrication de la fonction
		    if ( node->startTime > listNodes[i]->startTime && node->stopTime < listNodes[i]->stopTime )
			{
			    sizeParents++;
			    tempParentTab = (void*)realloc(tempParentTab, sizeParents*sizeof(funcNode_t));
			    tempParentTab[sizeParents-1] = listNodes[i];
			}
		}

	    if (sizeParents > 0)
		{
		    addChild(tempParentTab[sizeParents-1], (void*)node);
		    addParent(node, tempParentTab[sizeParents-1]);
		}
	}

    return 0;
}

/*
 * Calcul du temps exclusif
 */
static int computeExclTime(funcNode_t * node, funcNode_t * nodeParent)
{

    // On soustrait le temps inclusif du node parent par celui des nodes enfants
    // pour obtenir le temps exclusif
    nodeParent->timeExcl = nodeParent->timeExcl - node->timeInc;

    fprintf(pFile,"nodeParent %s exclusive time: %d\n",nodeParent->funcName, nodeParent->timeExcl);

    return 1;
}

/*
 * Parcours et affichage de l'arbre de fonctions
 */
static int parcoursPrefix(funcNode_t *node, int index, char *bufferPrint)
{
    int i;

    funcNode_t *tempParent = node->fNodeParent;

    if(node->numChilds > 0)
	{
	    for (i = index; i <= node->numChilds; i++)
		{
		    funcNode_t *tempChild = (funcNode_t*)malloc(sizeof(funcNode_t));
		    tempChild = node->funcChilds[i];

		    strcat(bufferPrint, "\"");
		    strcat(bufferPrint, node->funcName);
		    strcat(bufferPrint, "\"");
		    strcat(bufferPrint," -> ");
		    strcat(bufferPrint, "\"");
		    strcat(bufferPrint, tempChild->funcName);
		    strcat(bufferPrint, "\"");
		    strcat(bufferPrint, " ");

		    if (tempChild->numChilds > 0)
			{
			    parcoursPrefix(node->funcChilds[i],1, bufferPrint);
			}
		}

	}
    else
	{
	    return 1;
	}

    return 0;
}

/*
 * Initialisation des noeuds
 */
int initListNodes()
{

    //Initialize output stream
    pFile = fopen(PATHOUTPUT, "w");

    sizeListNodes = 1;
    rankList=0;
    listNodes = (void*)malloc(sizeListNodes*sizeof(funcNode_t));

    g_root = malloc(sizeof(funcNode_t));
    strcpy(g_root->funcName, "root");

    listNodes[rankList] = g_root;

    //Init tabFuncName
    tabFuncNameSize = 0;
    tabFuncName = (char*)malloc(tabFuncNameSize*sizeof(char));

    //Init InstFunc
    sizeInstFuncs = 0;
    sizeOutFuncNodes = 0;
    tabInstFunc = (InstFunc_t **)malloc(sizeInstFuncs*sizeof(InstFunc_t));
    outFuncNodes = (funcNode_t **)malloc(sizeOutFuncNodes*sizeof(funcNode_t));

    return 0;
}

/*
 * Création d'un noeud
 */
int createNode(char name[], int startTime, int stopTime)
{
    funcNode_t *fNode;

    if (startTime > stopTime)
	{
	    //printf("startTime > stopTime! \n");
	    return -1;
	}

    fNode = malloc(sizeof(funcNode_t));
    strcpy(fNode->funcName, name);

    fNode->startTime = startTime;
    fNode->stopTime = stopTime;
    fNode->funcChilds = NULL;

    fNode->funcChilds = (void*)malloc(sizeof(funcNode_t));
    fNode->numChilds = 0;

    fNode->timeInc = fNode->stopTime-fNode->startTime;
    fNode->timeExcl = fNode->timeInc;

    fprintf(pFile,"function %s inclusive time: %d\n",fNode->funcName, fNode->timeInc);

    checkNodeParent(fNode);

    int returnFct = addListNode(fNode);

    if (returnFct == -1)
	{
	    return -1;
	}

    funcNode_t *fTempNodeParent = fNode->fNodeParent;

    if (fNode->fNodeParent != NULL)
	{
	    computeExclTime(fNode, fNode->fNodeParent);
	}

    sizeOutFuncNodes++;
    outFuncNodes = (void*)realloc(outFuncNodes, sizeOutFuncNodes*sizeof(funcNode_t));
    outFuncNodes[sizeOutFuncNodes-1] = fNode;

    return 0;
}

/*
 * Gestion des instances
 */
int manageInstances()
{
    //Manage instances

    int i, k;

    for (i = 0; i < sizeOutFuncNodes; i++)
	{

	    //Test d'instance présente
	    int ret = 0;

	    for (k = 0; k < sizeInstFuncs; k++)
		{
		    //Si la fonction existe déjà dans le tableau d'instances, alors on rajoute l'instance

		    if (strcmp(outFuncNodes[i]->funcName, tabInstFunc[k]->funcName) == 0)
			{
			    tabInstFunc[k]->nbInsts++;

			    tabInstFunc[k]->tabTimeExcl = (int *)realloc(tabInstFunc[k]->tabTimeExcl, (tabInstFunc[k]->nbInsts)*sizeof(int));
			    tabInstFunc[k]->tabTimeIncl = (int *)realloc(tabInstFunc[k]->tabTimeIncl, (tabInstFunc[k]->nbInsts)*sizeof(int));

			    tabInstFunc[k]->tabTimeExcl[tabInstFunc[k]->nbInsts-1] = outFuncNodes[i]->timeExcl;
			    tabInstFunc[k]->tabTimeIncl[tabInstFunc[k]->nbInsts-1] = outFuncNodes[i]->timeInc;

			    k = sizeInstFuncs;

			    ret = 1;
			}
		}

	    if (ret == 0)
		{
		    sizeInstFuncs++;

		    tabInstFunc = (InstFunc_t**)realloc(tabInstFunc, sizeInstFuncs*sizeof(InstFunc_t));
		    tabInstFunc[sizeInstFuncs-1] = (InstFunc_t*)malloc(sizeof(InstFunc_t));

		    strcpy(tabInstFunc[sizeInstFuncs-1]->funcName, outFuncNodes[i]->funcName);

		    tabInstFunc[sizeInstFuncs-1]->nbInsts = 1;
		    int tempNbInsts = tabInstFunc[sizeInstFuncs-1]->nbInsts;

		    tabInstFunc[sizeInstFuncs-1]->tabTimeExcl = (int*)malloc(tabInstFunc[sizeInstFuncs-1]->nbInsts*sizeof(int));
		    tabInstFunc[sizeInstFuncs-1]->tabTimeIncl = (int*)malloc(tabInstFunc[sizeInstFuncs-1]->nbInsts*sizeof(int));

		    tabInstFunc[sizeInstFuncs-1]->tabTimeExcl[tempNbInsts-1] = outFuncNodes[i]->timeExcl;
		    tabInstFunc[sizeInstFuncs-1]->tabTimeIncl[tempNbInsts-1] = outFuncNodes[i]->timeInc;

		    tabInstFunc[sizeInstFuncs-1]->nbloads = 0;
		    tabInstFunc[sizeInstFuncs-1]->nbstores = 0;
		}
	}

    return 0;
}

/*
 * Vérification des instances
 */
int checkInstances()
{
    //printf("****Begin checkInstances****\n");

    int i, j;

    //printf("Taille InstFuncs: %d\n", sizeInstFuncs);

    for(i = 0; i < sizeInstFuncs; i++)
	{
	    //printf("Function names: %s\n", tabInstFunc[i]->funcName);

	    for (j = 0; j < tabInstFunc[i]->nbInsts; j++)
		{
		    //printf("instance %d of function %s 's exclusive time: %d\n", j, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeExcl[j]);
		    //printf("instance %d of function %s 's inclusive time: %d\n", j, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeIncl[j]);
		}
	}

    //printf("****End checkInstances****\n");

    return 0;
}

//************************Static analysis**********************//

/*
 * Calcul du nombre total de loads
 * pour une fonction
 */
int addStaticLoad(int nbloads)
{
    tabStaticFunc[sizeStaticFunc-1]->nbloads += nbloads;
    return 0;
}

/*
 * Calcul du nombre total de stores
 * pour une fonction
 */
int addStaticStore(int nbstores)
{
    tabStaticFunc[sizeStaticFunc-1]->nbstores += nbstores;
    return 0;
}

/*
 * Ajout d'un élément de la
 * structure fonction statiques
 */
int addStaticFuncName(char *funcName)
{
    //printf("***Begin createStaticFunc()***\n");

    sizeStaticFunc++;
    tabStaticFunc = (StaticFunc_t**)realloc(tabStaticFunc, sizeStaticFunc*sizeof(StaticFunc_t));

    tabStaticFunc[sizeStaticFunc-1] = (StaticFunc_t*)malloc(sizeof(StaticFunc_t));
    strcpy(tabStaticFunc[sizeStaticFunc-1]->funcName, funcName);

    tabStaticFunc[sizeStaticFunc-1]->nbloads = 0;
    tabStaticFunc[sizeStaticFunc-1]->nbstores = 0;

    //printf("***End createStaticFunc()***\n");
    return 0;
}

/*
 * Sérialisation du résultat de l'anlayse des fonctions
 */
int printInstsFuncs()
{
    FILE *profInst = fopen(PATHINST, "w");
    int i, j;

    for (i = 0; i < sizeInstFuncs; i++)
	{
	    fprintf(profInst, "%s", tabInstFunc[i]->funcName);
	    fprintf(profInst, " %d", tabInstFunc[i]->nbloads);
	    fprintf(profInst, " %d", tabInstFunc[i]->nbstores);
	    fprintf(profInst, " %d", tabInstFunc[i]->nbInsts);

	    for (j = 0; j < tabInstFunc[i]->nbInsts; j++)
		{
		    fprintf(profInst, " %d", tabInstFunc[i]->tabTimeIncl[j]);
		    fprintf(profInst, " %d", tabInstFunc[i]->tabTimeExcl[j]);
		}

	    fprintf(profInst, "\n");
	}

    return 0;
}

/*
 * Initialisation du tableau de fonctions statiques
 */
int initStaticFunc()
{
    //printf("***Begin initStaticFunc()***\n");

    sizeStaticFunc = 0;
    tabStaticFunc = (StaticFunc_t**)malloc(sizeStaticFunc*sizeof(StaticFunc_t));

    //printf("***End initStaticFunc()***\n");

    return 0;
}

/*
 * Vérification des données statiques reçues
 */
int checkStaticFunc()
{
    //printf("***Begin checkStaticFunc()***\n");
    int i;

    for (i = 0; i < sizeStaticFunc; i++)
	{
	    //printf("StaticFunc %d funcName: %s\n", i, tabStaticFunc[i]->funcName);
	    //printf("StaticFunc %d funcName length: %d\n", i, strlen(tabStaticFunc[i]->funcName));
	    //printf("StaticFunc %d nbre loads: %d\n", i, tabStaticFunc[i]->nbloads);
	    //printf("StaticFunc %d nbre stores: %d\n", i, tabStaticFunc[i]->nbstores);
	}

    //printf("***End checkStaticFunc()***\n");

    return 0;
}

/*
 * Corrélation entre les données statiques et dynamiques
 */
int correlateDynStatic()
{
    //printf("***Begin correlateDynStatic()***\n");
    int i, j;

    for (i = 0; i < sizeInstFuncs; i++)
	{
	    //printf("before correlation..\n");
	    //printf("number of loads: %d\n", tabInstFunc[i]->nbloads);
	    //printf("number of stores: %d\n", tabInstFunc[i]->nbstores);
	}

    for (i = 0; i < sizeInstFuncs; i++)
	{

	    for (j = 0; j < sizeStaticFunc; j++)
		{
		    //printf("tabStaticFunc index %d name: %s\n",j, tabStaticFunc[j]->funcName);

		    if (strcmp(tabStaticFunc[j]->funcName, tabInstFunc[i]->funcName) == 0)
			{
			    tabInstFunc[i]->nbloads = tabStaticFunc[j]->nbloads;
			    tabInstFunc[i]->nbstores = tabStaticFunc[j]->nbstores;
			}
		}
	}

    //printf("***End correlateDynStatic()***\n");

    return 0;
}

/*
 * Affichages des statistiques des fonctions
 */
int printFuncStats()
{
    //printf("***Begin printFuncStats()***\n");

    int i;
    FILE *profStats = fopen(PATHSTATS, "w");

    for (i = 0; i < sizeInstFuncs; i++)
	{
	    fprintf(profStats, "function %s\n", tabInstFunc[i]->funcName);
	    fprintf(profStats, "inclusive time: %d\n", tabInstFunc[i]->tabTimeIncl[0]);
	    fprintf(profStats, "exclusive time: %d\n", tabInstFunc[i]->tabTimeExcl[0]);
	    fprintf(profStats, "number of loads: %d\n", tabInstFunc[i]->nbloads);
	    fprintf(profStats, "number of stores: %d\n", tabInstFunc[i]->nbstores);
	    fprintf(profStats, "\n");
	}

    //printf("***End printFuncStats()***\n");

    return 0;
}

/*
 * Sérialisation du graphe d'appel des fonctions
 */
int printCG(funcNode_t *node)
{
    //printf("***Begin printCG()***\n");

    strcat(bufferPrint, "digraph { ");

    parcoursPrefix(node, 1, bufferPrint);

    strcat(bufferPrint, "}");

    FILE *profCG = fopen(PATHCG, "w");

    fprintf(profCG, "%s\n", bufferPrint);
    fclose(profCG);

    //printf("***End printCG()***\n");

    return 0;
}

/*
 * Calcul des temps moyen d'exécution des load et store
 * Résolution de systèmes d'équations
 * à l'aide de la méthode de Cramer
 */
int computeLoadStore()
{
    //printf("***Begin computeLoadStore()***\n");
    float *tabValueLoad, *tabValueStore;
    int i;

    averageTimeLoad = 0.0;
    averageTimeStore = 0.0;

    tabValueLoad = (float*)malloc(sizeInstFuncs*sizeof(float));
    tabValueStore = (float*)malloc(sizeInstFuncs*sizeof(float));

    for (i = 1; i < sizeInstFuncs; i++)
	{
	    int loads1 = tabInstFunc[0]->nbloads;
	    int stores1 =tabInstFunc[0]->nbstores;
	    int loads2 = tabInstFunc[i]->nbloads;
	    int stores2 = tabInstFunc[i]->nbstores;
	    int time1 = tabInstFunc[i]->tabTimeExcl[0];
	    int time2 = tabInstFunc[i]->tabTimeExcl[0];

	    float diviseur = loads1*stores2-stores1*loads2;

	    // Résolution du système
	    if (diviseur != 0.0)
		{
		    tabValueLoad[i] = (time1*stores2-stores1*time2)/(loads1*stores2-stores1*loads2);
		    tabValueStore[i] = (loads1*time2-time1*loads2)/(loads1*stores2-stores1*loads2);
		}
	    else
		{
		    tabValueLoad[i] = 0.0;
		    tabValueStore[i] = 0.0;
		}

	    averageTimeLoad += tabValueLoad[i];
	    averageTimeStore += tabValueStore[i];
	}

    // Calcul du temps moyen des load / store
    averageTimeLoad /= sizeInstFuncs-1;
    averageTimeStore /= sizeInstFuncs-1;

    //printf("***End computeLoadStore()***\n");

    return 0;
}
