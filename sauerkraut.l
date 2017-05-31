%{
#include "tokens.h"
%}
alpha	[a-zA-Z]
digit	[0-9]
%%
var				return VARIABLE;
{alpha}({alpha}|{digit})*	return IDENTIFIER;
{digit}+			return NUMBER;
"({alpha}|{digit})*"		return STRING;
