%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
\n  {printf("\n");}
appel {printf("call "); return CALL;}
fonction {printf("function "); return FUNCTION;}
"'"[0-9a-zA-Z]+"'" {printf("name "); yylval = yytext; return NAME;}
[0-9]+ {printf("time"); yylval = atoi(yytext); return TIME;}
entree {printf("enter "); return ENTER;}
cycle {printf("cycle "); return CYCLE;}
sortie {printf("exit "); return EXIT;}
%%

int yywrap(void) {
    return 1;
}
