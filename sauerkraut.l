%{
#include "y.tab.h"
#include <stdlib.h>
%}

L	[a-zA-Z_]
D	[0-9]

%%

[\t \n]
var			return VARKW;
function		return FUNKW;
end			return END;
while			return WHILEKW;
{L}({L}|{D})*		yylval.s = yytext; return ID;
[+-]?{D}+		yylval.i = atoi(yytext); return INTEGER;
L?\"(\\.|[^\\"])*\"	yylval.s = yytext; return STRING;
"<="			return LE;
">="			return GE;
"=="			return EQ;
"!="			return NE;
"||"			return OR;
"&&"			return AND;
.			return yytext[0];
%%
