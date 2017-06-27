%{
#include "node.h"
#include "sauerkraut.hpp"
#include <stdlib.h>

char * token = NULL;

%}

L	[a-zA-Z_]
D	[0-9]
%option noyywrap
%option yylineno
%%

[\t \n]
var			return VARKW;
function		return FUNKW;
end			return END;
while			return WHILEKW;
extern			return EXTERNKW;
{L}({L}|{D})*		yylval.s = yytext; return ID;
[+-]?{D}+		yylval.i = atoi(yytext); return INTEGER;
L?\"(\\.|[^\\"])*\"	{token = strtok(yytext,"\""); strtok(0,"\""); yylval.s = strdup(token); token = NULL; return STRING;}
"<="			return LE;
">="			return GE;
"=="			return EQ;
"!="			return NE;
"||"			return OR;
"&&"			return AND;
.			return yytext[0];
%%
