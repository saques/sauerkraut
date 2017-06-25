%{
#include "y.tab.h"
%}

L	[a-zA-Z_]
D	[0-9]

%%

[\t \n]
var			return VARKW;
function		return FUNKW;
end			return END;
{L}({L}|{D})*		return ID;
[+-]?{D}+		return INTEGER;
L?\"(\\.|[^\\"])*\"	return STRING;
"<="			return LE;
">="			return GE;
"=="			return EQ;
"!="			return NE;
"||"			return OR;
"&&"			return AND;
.			return yytext[0];
%%
