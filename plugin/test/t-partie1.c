#include <stdlib.h>

#pragma myproof instrumente (toto, main)

#define N 100000
#define M N

static int tab[N] = {1};

int toto (int *tab)
{
    int sum = tab[0] + tab[1] + tab[2];
    int i;

    for (i=0; i<N; i++)
    	{
    	    sum += tab[i] + 3 * tab[i+1];
    	    tab[i+1] = sum;
    	}

    sum += tab[0] + tab[1];

    for (i=0; i<M; i++)
    	{
    	    sum += tab[i] + 3 * tab[i+1] + 4 * tab[i-1];
    	    tab[i+1] = sum;
    	    tab[i+2] = 2*sum;
    	    tab[i+3] = 3*sum;
    	}
}

int main()
{
    toto(tab);
    return 0;
}
