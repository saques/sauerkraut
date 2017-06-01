%{
#include "tokens.h"
%}
alpha	[a-zA-Z]
digit	[0-9]
%%
var				{return VARIABLE;}
{alpha}({alpha}|{digit})*	{return IDENTIFIER;}
{digit}+			{return NUMBER;}
"({alpha}|{digit})*"		{return STRING;}
[				{return ARRAY_BEGIN;}
]				{return ARRAY_END;}
,				{return COMMA;}
{				{return OBJECT_BEGIN;}
}				{return OBJECT_END;}
:				{return COLON;}
function			{return FUNCTION;}
while				{return WHILE;}
if				{return IF;}
end				{return END;}
==				{return EQ;}
<				{return LT;}
>				{return GT;}
<=				{return LE;}
>=				{return GE;}
!				{return NOT;}
@				{return ANONYMOUS_FUNCTION;}
