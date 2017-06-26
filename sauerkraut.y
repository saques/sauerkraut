%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <deque>

void line(char * s);

char BUFFER[1024];
BlockNode * programStart;
extern int yylex();
extern int yylineno;

void yyerror(const char * s){
	std::fprintf(stderr, ">>>Error: %s at line %d \n", s, yylineno);
	std::exit(1);
}


%}
%define parse.error verbose
%union
{
	Node *node;
	StatementNode * st;
	ExpressionNode * expr;
	BlockNode *block;
	IdentifierNode *ident;
	VariableDeclarationNode *var_decl;
	std::deque<VariableDeclarationNode*> *arg_list;
	std::deque<ExpressionNode*> *expr_list;
	int i;
	char * s;
}
%token ID INTEGER STRING VARKW FUNKW END LE GE EQ NE OR AND WHILEKW

%type <st>	    FUNC VAR EXPR
%type <ident> 	IDENT
%type <block> 	ST BLOCK
%type <arg_list> ARGS ARGSET
%type <expr>	INSTR VALUE INT
%type <expr_list> PASSEDARGS

%type <i> INTEGER STRING
%type <s> ID

%right "="
%left OR AND
%left '>' '<' LE GE EQ NE
%left '+' '-'
%left '*' '/'
%left '!'
%%

S		: ST
		{
			programStart = $1;
		}
		;

ST		: EXPR ST
		{
			$$ = $2;
			$2->statements.push_front($<st>1);
		}
		| FUNC ST
		{
			$$ = $2;
			$2->statements.push_front($<st>1);
		}
		| VAR ST
		{
			$$ = $2;
			$2->statements.push_front($<st>1);
		}
		| /*empty*/
		{
			$$ = new BlockNode();
		}
		;

EXPR	: INSTR
		{
			$$ = new ExpressionStatementNode(*$1);
		}
		;

VAR		:  VARKW IDENT
		{
			$$ = new VariableDeclarationNode(*$2, 0);
		}
		| VARKW IDENT '=' VALUE
		{
			$$ = new VariableDeclarationNode(*$2, $4);
		}
		;

FUNC	: FUNKW IDENT ARGS '{' ST '}'
		{
			$$ = new FunctionDeclarationNode(*$2, $3, *$5);
		}
		;

ARGS	: '(' ARGSET ')'
		{
			$$ = $2;
		}
		| '(' ')'
		{
			$$ = new VariableList();
		}
		;

ARGSET		: IDENT
			{
				$$ = new VariableList();
				VariableDeclarationNode * node = new VariableDeclarationNode(*$1, 0);
				$$->push_front(node);
			}
			| IDENT ',' ARGSET
			{
				$$ = $3;
				VariableDeclarationNode * node = new VariableDeclarationNode(*$1, 0);
				$$->push_front(node);
			}
			;

IDENT		: ID
			{
				$$ = new IdentifierNode($1);
			}

INT			: INTEGER
			{
				$$ = new IntegerNode($1);
			}

BLOCK		: EXPR  BLOCK | /*empty*/ ;

INSTR		: ASSIGN ';'| CALL ';' | WHILE ;


CALL		:  IDENT '(' PASSEDARGS ')'
		 | IDENT '(' ')'
                   ;

PASSEDARGS	:  INSTR ',' PASSEDARGS
			{
				$$ = $3;
				$$->push_front($1);
			}
			| INSTR
			{
				$$ = new ExpressionList();
				$$->push_front($1);
			}
			| /*empty*/
			{
				$$ = new ExpressionList();
			}
			;


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

ASSIGN		: IDENT '=' I ;


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
		 | IDENT
		 | INT ;

OBJECT		: '{' KV_SET '}' ;

KV_SET		: KV KV_SET | KV | /*empty*/ ;

KV		: IDENT ':' VALUE ';' ;

ARRAY		: '['V_SET']' ;

V_SET		: VALUE ',' V_SET  | VALUE | /*empty*/ ;

VALUE		: INT | STRING | ARRAY | OBJECT ;

%%
void line(char * s){
	printf("%s\n",s);
}
int yywrap() {
	return 1;
}
/*main() {
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
}*/
