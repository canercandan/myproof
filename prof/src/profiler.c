#include <stdio.h>
#include "profiler.h"

int openFile(FILE *file, char *path)
{
    printf("****Begin openFile****\n");

    printf("path: %s\n", path);
    file = fopen(path, "w");

    printf("****End openFile****\n");

    return 0;
}

int closeFile(FILE *file)
{
    printf("****Begin closeFile****\n");
    fclose(file);
    printf("****End closeFile****\n");
    return 0;
}

///////////////////////////////////////////////////////////
// Ajout d'un noeud enfant                               //
///////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////
// Ajout d'un noeud parent                               //
///////////////////////////////////////////////////////////
int addParent(funcNode_t *fNode, funcNode_t *fParent)
{
    printf("****Begin addParent****\n");

    printf("node %s parent: %s\n", fNode->funcName, fParent->funcName);

    fNode->fNodeParent = fParent;

    printf("****End addParent****\n");
    return 0;
}


//////////////////////////////////////////////////////////
// Initialisation des noeuds                            //
//////////////////////////////////////////////////////////
int initListNodes()
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
    tabInstFunc = (InstFunc_t **)malloc(sizeInstFuncs*sizeof(InstFunc_t));
    outFuncNodes = (funcNode_t **)malloc(sizeOutFuncNodes*sizeof(funcNode_t));

    printf("****End initListNodes****\n");

    return 0;
}

////////////////////////////////////////////////////
// Ajout d'un noeud dans la liste                 //
////////////////////////////////////////////////////
int addListNode(funcNode_t * node)
{
    printf("****Begin addListNode****\n");
    printf("rankList: %d\n", rankList);

    if(node != NULL) {
	if(rankList >= 1) {
	    if(((node->startTime > listNodes[rankList-1]->startTime) && (node->stopTime < listNodes[rankList-1]->stopTime))
	       || ((node->startTime > listNodes[rankList-1]->stopTime) && (node->stopTime > listNodes[rankList-1]->stopTime))) {
		int i;
		sizeListNodes++;
		listNodes = (funcNode_t *)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
		listNodes[rankList] = node;
		rankList++;
	    }
	    else {
		fprintf(stderr, "Measure error\n");
		return -1;
	    }
	}
	else {
	    int i;
	    sizeListNodes++;
	    listNodes = (funcNode_t *)realloc(listNodes, sizeListNodes*sizeof(funcNode_t));
	    listNodes[rankList] = node;
	    addParent(node,root);
	    rankList++;
	}
    }

    printf("node startTime at rankList %d: %d\n", rankList, listNodes[rankList-1]->startTime);
    printf("rankList: %d\n", rankList);

    printf("****End addListNode*****\n");

    return 0;
}

//////////////////////////////////////////////////////////////
// Vérifier si le noeud précédent est un parent             //
//////////////////////////////////////////////////////////////
int checkNodeParent(funcNode_t * node)
{
    printf("****Begin CheckNodeParent****\n");
    int i;

    printf("check %s parent\n", node->funcName);
    printf("ranklist: %d\n", rankList);

    if(node == NULL) {
	printf("node NULL\n");
    }

    funcNode_t **tempParentTab;
    int sizeParents = 0;
    tempParentTab = (funcNode_t*)malloc(0*sizeof(funcNode_t));

    if(rankList == 0) {
	printf("ranklist 0: function name: %s\n", node->funcName);
	addChild(root, node);
	addParent(node, root);
    }

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

////////////////////////////////////////////////////////////////
// Calcul du temps exclusif                                   //
////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////
// Création d'un noeud                                                    //
////////////////////////////////////////////////////////////////////////////
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
    // outFuncNodes[sizeOutFuncNodes-1] = (funcNode_t*)malloc(sizeof(funcNode_t));
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

/////////////////////////////////////////////////////
// Gestion des instances                           //
/////////////////////////////////////////////////////
int manageInstances()
{

    //Manage instances
    printf("\n***Begin Instances management***\n");

    int i, j, k;

    for(i=0;i<sizeOutFuncNodes;i++) {

	printf("\n");
	printf("-----------------------\n");
	printf("outFuncNodes %d name: %s\n",i, outFuncNodes[i]->funcName);
	printf("outFuncNodes %d length name: %d\n", i, strlen(outFuncNodes[i]->funcName));
	printf("outFuncNodes %d inclusive time: %d\n", i, outFuncNodes[i]->timeInc);
	printf("outFuncNodes %d exclusive time: %d\n", i, outFuncNodes[i]->timeExcl);

	//Test d'instance présente
	//   int k;
	int ret = 0;
	//   printf("outFuncNodes %d name: %s\n",i, outFuncNodes[i]->funcName);

	for(k=0;k<sizeInstFuncs;k++) {

	    //Si la fonction existe déjà dans le tableau d'instances, alors on rajoute l'instance
	    //printf("tab Instance function name: %s\n", tabInstFunc[j]->funcName);

	    if(strncmp(outFuncNodes[i]->funcName, tabInstFunc[k]->funcName, 6) == 0) {

		printf("Add an instance of function\n");

		printf("function name: %s\n", tabInstFunc[k]->funcName);
		//nbInsts++;
		tabInstFunc[k]->nbInsts++;
		printf("nb Instances: %d\n", tabInstFunc[k]->nbInsts);
		tabInstFunc[k]->tabTimeExcl = (int *)realloc(tabInstFunc[k]->tabTimeExcl, (tabInstFunc[k]->nbInsts)*sizeof(int));
		tabInstFunc[k]->tabTimeIncl = (int *)realloc(tabInstFunc[k]->tabTimeIncl, (tabInstFunc[k]->nbInsts)*sizeof(int));

		printf("function node exclusive time: %d\n", outFuncNodes[i]->timeExcl);
		printf("function node inclusive time: %d\n", outFuncNodes[i]->timeInc);

		tabInstFunc[k]->tabTimeExcl[tabInstFunc[k]->nbInsts-1] = outFuncNodes[i]->timeExcl;
		tabInstFunc[k]->tabTimeIncl[tabInstFunc[k]->nbInsts-1] = outFuncNodes[i]->timeInc;

		k=sizeInstFuncs;

		//     return 1;
		ret = 1;
	    }
	}

	if(ret == 0) {

	    printf("\nAdd InstFunc element\n");
	    sizeInstFuncs++;
	    printf("size InstFuncs tab: %d\n", sizeInstFuncs);
	    tabInstFunc = (InstFunc_t**)realloc(tabInstFunc, sizeInstFuncs*sizeof(InstFunc_t));
	    tabInstFunc[sizeInstFuncs-1] = (InstFunc_t*)malloc(sizeof(InstFunc_t));

	    strcpy(tabInstFunc[sizeInstFuncs-1]->funcName, outFuncNodes[i]->funcName);

	    printf("Length name outFuncNodes %d: %d\n", i, strlen(outFuncNodes[i]->funcName));
	    printf("copy name %s\n", outFuncNodes[i]->funcName);
	    printf("InstFunc %d name: %s\n", i, outFuncNodes[i]->funcName);

	    tabInstFunc[sizeInstFuncs-1]->nbInsts = 1;
	    int tempNbInsts = tabInstFunc[sizeInstFuncs-1]->nbInsts;

	    tabInstFunc[sizeInstFuncs-1]->tabTimeExcl = (int*)malloc(tabInstFunc[sizeInstFuncs-1]->nbInsts*sizeof(int));
	    tabInstFunc[sizeInstFuncs-1]->tabTimeIncl = (int*)malloc(tabInstFunc[sizeInstFuncs-1]->nbInsts*sizeof(int));

	    //    tabInstFunc[sizeInstFuncs-1]->tabTimeExcl = (int*)realloc(tabInstFunc[sizeInstFuncs-1]->tabTimeExcl, (tabInstFunc[sizeInstFuncs-1]->nbInsts)*sizeof(int));
	    //    tabInstFunc[sizeInstFuncs-1]->tabTimeIncl = (int*)realloc(tabInstFunc[sizeInstFuncs-1]->tabTimeIncl, (tabInstFunc[sizeInstFuncs-1]->nbInsts)*sizeof(int));

	    printf("outFuncNodes %d inclusive time: %d\n", i, outFuncNodes[i]->timeInc);
	    printf("outFuncNodes %d exclusive time: %d\n", i, outFuncNodes[i]->timeExcl);

	    tabInstFunc[sizeInstFuncs-1]->tabTimeExcl[tempNbInsts-1] = outFuncNodes[i]->timeExcl;
	    tabInstFunc[sizeInstFuncs-1]->tabTimeIncl[tempNbInsts-1] = outFuncNodes[i]->timeInc;

	    tabInstFunc[sizeInstFuncs-1]->nbloads = 0;
	    tabInstFunc[sizeInstFuncs-1]->nbstores = 0;
	    //
	    //    printf("instFuncs %d inclusive time: %d\n", i, tabInstFunc[sizeInstFuncs-1]->tabTimeIncl[tabInstFunc[sizeInstFuncs-1]->nbInsts]);
	    //    printf("instFuncs %d exclusive time: %d\n", i, tabInstFunc[sizeInstFuncs-1]->tabTimeExcl[tabInstFunc[sizeInstFuncs-1]->nbInsts]);

	}
    }

    for(i=0;i<sizeInstFuncs;i++) {
	printf("instFuncs %d function name %s length: %d\n", i, tabInstFunc[i]->funcName, strlen(tabInstFunc[i]->funcName));
	printf("instFuncs %d function name %s inclusive time: %d\n", i, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeIncl[0]);
	printf("instFuncs %d function name %s exclusive time: %d\n", i, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeExcl[0]);
    }

    printf("\n***End Instances management***\n");

    return 0;
}


////////////////////////////////////////////////////////
// Vérification des instances                         //
////////////////////////////////////////////////////////
int checkInstances()
{
    printf("****Begin checkInstances****\n");

    int i,j;

    printf("Taille InstFuncs: %d\n", sizeInstFuncs);

    for(i=0;i<sizeInstFuncs;i++) {
	printf("Function names: %s\n", tabInstFunc[i]->funcName);

	//   if(tabInstFunc[i]->nbInsts > 1) {
	//    printf("%d instances of function %s\n", tabInstFunc[i]->nbInsts, tabInstFunc[i]->funcName);

	for(j=0;j<tabInstFunc[i]->nbInsts;j++) {
	    printf("instance %d of function %s 's exclusive time: %d\n", j, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeExcl[j]);
	    printf("instance %d of function %s 's inclusive time: %d\n", j, tabInstFunc[i]->funcName, tabInstFunc[i]->tabTimeIncl[j]);
	}
	//   }
    }

    printf("****End checkInstances****\n");

    return 0;
}


//************************Static analysis**********************//

//////////////////////////////////////
// Calcul du nombre total de loads  //
// pour une fonction                //
//////////////////////////////////////
int addStaticLoad(int nbloads)
{
    tabStaticFunc[sizeStaticFunc-1]->nbloads += nbloads;
    return 0;
}

///////////////////////////////////////
// Calcul du nombre total de stores  //
// pour une fonction                 //
///////////////////////////////////////
int addStaticStore(int nbstores)
{
 tabStaticFunc[sizeStaticFunc-1]->nbstores += nbstores;
 return 0;
}

///////////////////////////////////////
// Ajout d'un élément de la          //
// structure fonction statiques      //
///////////////////////////////////////
int addStaticFuncName(char * funcName)
{
    printf("***Begin createStaticFunc()***\n");

    sizeStaticFunc++;
    //     tabInstFunc = (InstFunc_t*)realloc(tabInstFunc, sizeInstFuncs*sizeof(InstFunc_t));
    tabStaticFunc = (StaticFunc_t**)realloc(tabStaticFunc, sizeStaticFunc*sizeof(StaticFunc_t));

    tabStaticFunc[sizeStaticFunc-1] = (StaticFunc_t*)malloc(sizeof(StaticFunc_t));
    strcpy(tabStaticFunc[sizeStaticFunc-1]->funcName, funcName);
    printf("tab static func name index %d: %s\n",  sizeStaticFunc-1, tabStaticFunc[sizeStaticFunc-1]->funcName);
    tabStaticFunc[sizeStaticFunc-1]->nbloads = 0;
    tabStaticFunc[sizeStaticFunc-1]->nbstores = 0;

    printf("***End createStaticFunc()***\n");
    return 0;
}

///////////////////////////////////////////////////////////
// Sérialisation du résultat de l'anlayse des fonctions  //
///////////////////////////////////////////////////////////
int printInstsFuncs()
{
    FILE *profInst = fopen(PATHINST, "w");
    int i, j;

    char * strFunc = "";
    char * bufferInst = "";

    for(i=0;i<sizeInstFuncs;i++) {
	printf("tabInstFunc %d\n", i);
	//   strFunc = "";
	fprintf(profInst, " %s", tabInstFunc[i]->funcName);
	//   strcat(bufferInst, tabInstFunc[i]->funcName);
	//   strcat(strFunc, tabInstFunc[i]->nbInsts);
	fprintf(profInst, " %d", tabInstFunc[i]->nbInsts);
	fprintf(profInst, " nloads %d", tabInstFunc[i]->nbloads);
	fprintf(profInst, " nstores %d", tabInstFunc[i]->nbstores);
	//     printf("tabInstFunc %d\n", i);
	//  if(tabInstFunc[i]->nbInsts == 1
	for(j=0;j<tabInstFunc[i]->nbInsts;j++) {
	    printf("Instance %d\n", j);
	    fprintf(profInst, " inclusif ");
	    fprintf(profInst, "%d", tabInstFunc[i]->tabTimeIncl[j]);
	    fprintf(profInst, " exclusif ");
	    fprintf(profInst, "%d", tabInstFunc[i]->tabTimeExcl[j]);
	    //   strcat(bufferInst, " inclusif ");
	    //   strcat(bufferInst, tabInstFunc[i]->tabTimeIncl[j]);
	    //   strcat(bufferInst, " exclusif ");
	    //   strcat(bufferInst, tabInstFunc[i]->tabTimeExcl[j]);
	    //   strcat(bufferInst, " ");
	    //   strcat(bufferInst, "inclusif %d exclusif %d ",tabInstFunc[i]->tabTimeIncl[j], tabInstFunc[i]->tabTimeExcl[j]);
	}
	fprintf(profInst, "\n");
	// fprintf(profInst,"%s\n", bufferInst);*
    }

    fprintf(profInst, "average time of load: %f\n", averageTimeLoad);
    fprintf(profInst, "average time of store: %f\n", averageTimeStore);

    return 0;
}


////////////////////////////////////////////////////////
// Initialisation du tableau de fonctions statiques   //
////////////////////////////////////////////////////////
int initStaticFunc()
{
    printf("***Begin initStaticFunc()***\n");

    sizeStaticFunc = 0;
    tabStaticFunc = (StaticFunc_t**)malloc(sizeStaticFunc*sizeof(StaticFunc_t));

    printf("***End initStaticFunc()***\n");

    return 0;
}


int checkStaticFunc()
{
    printf("***Begin checkStaticFunc()***\n");
    int i;

    for(i=0;i<sizeStaticFunc;i++) {
	printf("StaticFunc %d funcName: %s\n", i, tabStaticFunc[i]->funcName);
	printf("StaticFunc %d funcName length: %d\n", i, strlen(tabStaticFunc[i]->funcName));
	printf("StaticFunc %d nbre loads: %d\n", i, tabStaticFunc[i]->nbloads);
	printf("StaticFunc %d nbre stores: %d\n", i, tabStaticFunc[i]->nbstores);
    }

    printf("***End checkStaticFunc()***\n");

    return 0;
}

//////////////////////////////////////////
// Corrélation entre                    //
// les données statiques et dynamiques  //
//////////////////////////////////////////
int correlateDynStatic()
{
    printf("***Begin correlateDynStatic()***\n");
    int i, j;

    printf("sizeInstFuncs: %d\n", sizeInstFuncs);
    printf("sizeStaticFunc: %d\n", sizeStaticFunc);

    for(i=0;i<sizeInstFuncs;i++) {
	printf("before correlation..\n");
	printf("number of loads: %d\n", tabInstFunc[i]->nbloads);
	printf("number of stores: %d\n", tabInstFunc[i]->nbstores);
    }

    for(i=0;i<sizeInstFuncs;i++) {
	printf("tabInstFunc index %d name: %s\n",i, tabInstFunc[i]->funcName);
	for(j=0;j<sizeStaticFunc;j++) {
	    printf("tabStaticFunc index %d name: %s\n",j, tabStaticFunc[j]->funcName);
	    if(strncmp(tabStaticFunc[j]->funcName, tabInstFunc[i]->funcName, 6) == 0) {
		printf("Match between dynamic and static trace of function %s in rank %d\n", tabInstFunc[i]->funcName, i);
		tabInstFunc[i]->nbloads = tabStaticFunc[j]->nbloads;
		tabInstFunc[i]->nbstores = tabStaticFunc[j]->nbstores;

		printf("number of loads: %d\n", tabInstFunc[i]->nbloads);
		printf("number of stores: %d\n", tabInstFunc[i]->nbstores);
	    }
	}
    }

    printf("***End correlateDynStatic()***\n");

    return 0;
}

////////////////////////////////////////
// Affichages des statistiques        //
// des fonctions                      //
////////////////////////////////////////
int printFuncStats() {

    printf("***Begin printFuncStats()***\n");

    int i;
    FILE *profStats = fopen(PATHSTATS, "w");

    for(i=0;i<sizeInstFuncs;i++) {
	fprintf(profStats, "function %s\n", tabInstFunc[i]->funcName);
	fprintf(profStats, "inclusive time: %d\n", tabInstFunc[i]->tabTimeIncl[0]);
	fprintf(profStats, "exclusive time: %d\n", tabInstFunc[i]->tabTimeExcl[0]);
	fprintf(profStats, "number of loads: %d\n", tabInstFunc[i]->nbloads);
	fprintf(profStats, "number of stores: %d\n", tabInstFunc[i]->nbstores);
	fprintf(profStats, "\n");
    }

    printf("***End printFuncStats()***\n");

    return 0;
}

///////////////////////////////////////////////////////////
// Parcours et affichage de l'arbre de fonctions         //
///////////////////////////////////////////////////////////
int parcoursPrefix(funcNode_t *node, int index, char *bufferPrint)
{
    printf("***Begin parcoursPrefix()***\n");
    int i, j;
    printf("index: %d\n", index);

    funcNode_t *tempParent = node->fNodeParent;

    printf("function name: %s\n", node->funcName);
    printf("function parent: %s\n", tempParent->funcName);
    printf("function nb childs: %d\n", node->numChilds);

    if(node->numChilds > 0) {
	/*  printf(" node has childs\n");*/

	for(i=index;i<=node->numChilds;i++) {
	    /*   printf("i: %d\n", i);*/
	    funcNode_t *tempChild = (funcNode_t*)malloc(sizeof(funcNode_t));
	    tempChild = node->funcChilds[i];

	    printf("%s -> %s\n", node->funcName, tempChild->funcName);
	    printf("Concatenation\n");
	    strcat(bufferPrint, "\"");
	    strcat(bufferPrint, node->funcName);
	    strcat(bufferPrint, "\"");
	    strcat(bufferPrint," -> ");
	    strcat(bufferPrint, "\"");
	    strcat(bufferPrint, tempChild->funcName);
	    strcat(bufferPrint, "\"");
	    strcat(bufferPrint, " ");

	    if(tempChild->numChilds > 0) {
		/*    printf("nb childs of children\n");*/
		parcoursPrefix(node->funcChilds[i],1, bufferPrint);
	    }
	}

    }
    else {


	return 1;
    }

    printf("bufferPrint: %s\n", bufferPrint);

    printf("***End parcoursPrefix()***\n");

    return 0;
}

int printCG(funcNode_t *node)
{
    printf("***Begin printCG()***\n");

    strcat(bufferPrint, "digraph { ");

    parcoursPrefix(node, 1, bufferPrint);

    strcat(bufferPrint, "}");

    printf("bufferPrint: %s\n", bufferPrint);

    FILE *profCG = fopen(PATHCG, "w");

    fprintf(profCG, bufferPrint);
    fclose(profCG);

    printf("***End printCG()***\n");

    return 0;
}

///////////////////////////////////////////////////////////////
//  Calcul des temps moyen d'exécution des load et store     //
//  Résolution de systèmes d'équations			     //
//  à l'aide de la méthode de Cramer   			     //
///////////////////////////////////////////////////////////////
int computeLoadStore()
{
    printf("***Begin computeLoadStore()***\n");
    float *tabValueLoad, *tabValueStore;
    int ValLoad, ValStore;
    int i;

    averageTimeLoad = 0.0;
    averageTimeStore = 0.0;

    tabValueLoad = (float*)malloc(sizeInstFuncs*sizeof(float));
    tabValueStore = (float*)malloc(sizeInstFuncs*sizeof(float));


    for(i=1;i<sizeInstFuncs;i++) {

	printf("Iterate tabInstFunc %d\n", i);
	int loads1 = tabInstFunc[0]->nbloads;
	int stores1 =tabInstFunc[0]->nbstores;
	int loads2 = tabInstFunc[i]->nbloads;
	int stores2 = tabInstFunc[i]->nbstores;
	int time1 = tabInstFunc[i]->tabTimeExcl[0];
	int time2 = tabInstFunc[i]->tabTimeExcl[0];

	printf("Calculate load and store time at %d\n", i);

	float diviseur = loads1*stores2-stores1*loads2;

	printf("diviseur: %f\n", diviseur);

	//Résolution du système
	if(diviseur != 0.0) {
	    tabValueLoad[i] = (time1*stores2-stores1*time2)/(loads1*stores2-stores1*loads2);
	    tabValueStore[i] = (loads1*time2-time1*loads2)/(loads1*stores2-stores1*loads2);
	}
	else {
	    tabValueLoad[i] = 0.0;
	    tabValueStore[i] = 0.0;
	}

	printf("load at %d rank executed in %f cycles\n", i, tabValueLoad[i]);
	printf("store at %d rank executed in %f cycles\n", i, tabValueStore[i]);

	averageTimeLoad += tabValueLoad[i];
	averageTimeStore += tabValueStore[i];

    }

    //Calcul du temps moyen des load / store
    averageTimeLoad /= sizeInstFuncs-1;
    averageTimeStore /= sizeInstFuncs-1;

    printf("averageTimeLoad: %f cycles\n", averageTimeLoad);
    printf("averageTimeStore: %f cycles\n", averageTimeStore);

    printf("***End computeLoadStore()***\n");

    return 0;
}