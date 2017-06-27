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
%token ID INTEGER STRING VARKW FUNKW END LE GE EQ NE OR AND WHILEKW EXTERNKW

%type <st>	    FUNC VAR EXPR EXTERN_FUNC
%type <ident> 	IDENT
%type <block> 	ST BLOCK
%type <arg_list> ARGS ARGSET
%type <expr>	INSTR VALUE INT CALL STR
%type <expr_list> PASSEDARGS
%type <i> INTEGER EXTERN_FUNC_ARGS
%type <s> ID STRING

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
		| EXTERN_FUNC ST
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

EXTERN_FUNC		: EXTERNKW FUNKW IDENT '(' EXTERN_FUNC_ARGS ')'
				{
					$$ = new ExternalFunctionDeclarationNode(*$3, $5);
				}
				| EXTERNKW FUNKW IDENT '(' ')'
				{
					$$ = new ExternalFunctionDeclarationNode(*$3, 0);
				}
				;

EXTERN_FUNC_ARGS	: ID
					{
						$$ = 1;

					}
					| ID ',' EXTERN_FUNC_ARGS
					{
						$$ = 1 + $3;
					}

INT			: INTEGER
			{
				$$ = new IntegerNode($1);
			}
			
STR			: STRING
			{
				$$ = new StringNode($1);
			}

BLOCK		: EXPR  BLOCK | /*empty*/ ;

INSTR		: I | ASSIGN | CALL  | WHILE ;


CALL		:  IDENT '(' PASSEDARGS ')'
			{
				$$ = new FunctionCallNode(*$1, *$3);
			}
		 	| IDENT '(' ')'
			{
				ExpressionList *list = new ExpressionList();
				$$ = new FunctionCallNode(*$1, *list);
			}
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

VALUE		: INT | STR | ARRAY | OBJECT ;

%%
void line(char * s){
	printf("%s\n",s);
}
int yywrap() {
	return 1;
}
