%{
#include <stdio.h>
#include "parser.h"
%}

%%
\n			{return RETLINE;}
"appel à la "		{return CALL;}
"fonction "		{return FUNCTION;}
"'"[0-9a-zA-Z]+"'"	{yylval.str = yytext; return NAME;}
[0-9]+			{yylval.value = atoi(yytext); return NUMERIC;}
" entree cycle "	{return ENTERCYCLE;}
" sortie cycle "	{return EXITCYCLE;}
" load"			{return LOAD;}
" store"			{return STORE;}
" * "			{return MUL;}
%%

int yywrap(void) { return 1; }
