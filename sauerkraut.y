%{
#include <stdio.h>
#include <stdlib.h>

void line(char * s);

char BUFFER[1024];

%}
%define parse.error verbose
%union
{
	int i;
	char * s;
}
%type<s> id STRING VALUE
%type<i> INTEGER
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

EXPR		: VAR ';' | INSTR ;

VAR		:  VARKW id           {printf("void * %s = null;\n",$2) ;} 
                 | VARKW id '=' VALUE {printf("void * %s = %s;\n",$2,$4);}

FUNC		: FUNKW id ARGS BLOCK END;

ARGS		: '(' ARGSET ')' | '(' ')';

ARGSET		: id | id ',' ARGSET ;

BLOCK		: EXPR  BLOCK | /*empty*/ ;

INSTR		: ASSIGN ';'| CALL ';' | WHILE ;

CALL		:  id '(' PASSEDARGS ')'
		 | id '(' ')'
                   ;

PASSEDARGS	:  VALUE ',' PASSEDARGS
		 | ID ',' PASSEDARGS
		 | VALUE
		 | id;

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

ASSIGN		:  id '=' I 
		 | id '=' VALUE ;

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
		 | id   ;

OBJECT		: '{' KV_SET '}' ;

KV_SET		: KV KV_SET | KV | /*empty*/ ;

KV		: id ':' VALUE ';' ; 

ARRAY		: '['V_SET']' ;

V_SET		: VALUE ',' V_SET  | VALUE | /*empty*/ ;

VALUE		:  INTEGER { sprintf(BUFFER,"newObject(newInteger(%d),integerClass)",$1); $$ = BUFFER ;}; 
		 | STRING  { sprintf(BUFFER,"newObject(newString(%s),stringClass)",$1);   $$ = BUFFER ;};
		 | ARRAY 
		 | OBJECT ; 

id		: ID { $$ = strdup(yylval.s); } ;

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
	line("#include <String.h>");
	line("#include <Object.h>");

	//setup global variables and class singletons
	line("Class * integerClass = integerClass();");
	line("Class * methodClass = methodClass();");
	line("Class * stringClass = stringClass();");
	yyparse();
}
