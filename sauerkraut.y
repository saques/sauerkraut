%{
#include <stdio.h>
#include <stdlib.h>
%}
%define parse.error verbose
%token ID INTEGER STRING
%%

S	: ST {printf("Input Accepted\n"); exit(0);} ;

ST	: TEST ;

TEST	: OBJECT | ARRAY ; 

OBJECT	: '{' KV_SET '}' ;

KV_SET	: KV KV_SET | KV ;

KV	: ID ':' VALUE ';' ; 

ARRAY	: '['V_SET']' ;

V_SET   : VALUE ',' V_SET  | VALUE ;

VALUE	: INTEGER | STRING | ARRAY | OBJECT ; 

%%
int yywrap() {
	return 1;
}
void yyerror(char const *s){
	fprintf(stderr,"%s\n", s);
}
main() {
	yyparse();
}
