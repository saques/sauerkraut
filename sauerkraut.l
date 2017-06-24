%{
#include "y.tab.h"
%}

L	[a-zA-Z_]
D	[0-9]

%%
[\t \n]
{L}({L}|{D})*		return ID;
[+-]?{D}+		return INTEGER;
L?\"(\\.|[^\\"])*\"	return STRING;
.			return yytext[0];
%%
