%{
#include <stdio.h>
#include <stdlib.h>

void line(char * s);

%}
%define parse.error verbose
%union
{
	int i;
	char * s;
}
%token ID INTEGER STRING VARKW FUNKW END LE GE EQ NE OR AND
%right "="
%left OR AND
%left '>' '<' LE GE EQ NE
%left '+' '-'
%left '*' '/'
%left '!'
%%

S		: ST {printf("Input Accepted\n"); exit(0);} ;

ST		: EXPR ';' ST | ;

EXPR		: DECLR | INSTR ;

DECLR		: VAR | FUNC;

VAR		: VARKW ID | VARKW ID '=' VALUE ;

FUNC		: FUNKW ID ARGS BLOCK ;

ARGS		: '(' ARGSET ')' | '(' ')';

ARGSET		: ID | ID ',' ARGSET ;

BLOCK		: EXPR BLOCK | END ;

INSTR		: ASSIGN | I;

ASSIGN		: ID '=' I ;

I		:  I '+' I 
		 | I '-' I
		 | I '*' I
		 | I '/' I
		 | I '<' I
		 | I '>' I
		 | I LE I
		 | I GE I
		 | I EQ I
		 | I NE I
		 | I OR I
		 | I AND I
		 | I '+' '+' 
		 | I '-' '-'
		 | ID  
		 | INTEGER ;

OBJECT		: '{' KV_SET '}' ;

KV_SET		: KV KV_SET | KV | ;

KV		: ID ':' VALUE ';' ; 

ARRAY		: '['V_SET']' ;

V_SET		: VALUE ',' V_SET  | VALUE | ;

VALUE		: INTEGER | STRING | ARRAY | OBJECT ; 

%%
void line(char * s){
	printf("%s\n",s);
}
int yywrap() {
	return 1;
}
void yyerror(char const *s){
	fprintf(stderr,"%s\n", s);
}
main() {
	//INCLUDES
	line("#include <Method.h>");
	line("#include <Integer.h>");
	//setup global variables and class singletons
	line("Class * integerClass = integerClass()");
	line("Class * methodClass = methodClass()");
	yyparse();
}
