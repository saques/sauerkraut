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
%token ID INTEGER STRING VARKW FUNKW END LE GE EQ NE OR AND WHILEKW
%right "="
%left OR AND
%left '>' '<' LE GE EQ NE
%left '+' '-'
%left '*' '/'
%left '!'
%%

S		: ST ;

ST		: EXPR ST | FUNC ST | /*empty*/ ;

EXPR		: VAR | INSTR ;

VAR		:  VARKW ID 
                 | VARKW ID '=' VALUE ;

FUNC		: FUNKW ID ARGS BLOCK END;

ARGS		: '(' ARGSET ')' | '(' ')';

ARGSET		: ID | ID ',' ARGSET ;

BLOCK		: EXPR  BLOCK | /*empty*/ ;

INSTR		: ASSIGN ';'| CALL ';' | WHILE ;

CALL		:  ID '(' PASSEDARGS ')'
		 | ID '(' ')'
                   ;

PASSEDARGS	:  VALUE ',' PASSEDARGS
		 | ID ',' PASSEDARGS
		 | VALUE
		 | ID;

WHILE		: WHILEKW '(' I2 ')' BLOCK END;

I2		:  I '<' I
		 | I '>' I
		 | I LE I
		 | I GE I
		 | I EQ I
		 | I NE I
		 | I OR I
		 | I AND I
		 ;

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
		 | ID  
		 | INTEGER ;

OBJECT		: '{' KV_SET '}' ;

KV_SET		: KV KV_SET | KV | /*empty*/ ;

KV		: ID ':' VALUE ';' ; 

ARRAY		: '['V_SET']' ;

V_SET		: VALUE ',' V_SET  | VALUE | /*empty*/ ;

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
