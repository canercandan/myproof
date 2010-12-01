#include <stdio.h>

typedef struct {
char name[10];
int start_time;
int stop_time;
int time_inc;
int time_excl;
struct FuncNode * FuncChild[0];
}FuncNode;
