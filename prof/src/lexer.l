%{
#include <stdio.h>
#include "parser.h"
%}

%%
\n  {printf("\n"); return RETLINE;}
appel {printf("call "); return CALL;}
fonction {printf("function "); return FUNCTION;}
"'"[0-9a-zA-Z]+"'" {printf("name "); yylval.str = yytext; return NAME;}
[0-9]+ {printf("numeric "); yylval.value = atoi(yytext); return NUMERIC;}
"entree cycle" {printf("enter cycle "); return ENTERCYCLE;}
"sortie cycle" {printf("exit cycle "); return EXITCYCLE;}
load {return LOAD;}
store {return STORE;}
"*" {printf("mul "); return MUL;}
%%

int yywrap(void)
{
    return 1;
}
