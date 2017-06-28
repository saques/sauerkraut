%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <deque>
#define LOWER(a,b) new BinaryOperationNode(a, b,"lower")
#define GREATER(a,b) new BinaryOperationNode(a, b,"greater")
#define EQUAL(a,b) new BinaryOperationNode(a, b,"equal")
#define NOT(a) new UnaryOperationNode(a,std::string("not"))



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
	std::deque<ExpressionNode*> *arr_list;
	int i;
	char * s;
}
%token ID INTEGER STRING VARKW FUNKW END LE GE EQ NE OR AND WHILEKW EXTERNKW RETKW

%type <st>	    FUNC VAR EXPR EXTERN_FUNC RET
%type <ident> 	IDENT
%type <block> 	ST BLOCK
%type <arg_list> ARGS ARGSET
%type <expr>	INSTR VALUE INT CALL I STR ASSIGN ARRAY
%type <expr_list> PASSEDARGS
%type <i> INTEGER EXTERN_FUNC_ARGS
%type <s> ID STRING

%type <arr_list> V_SET


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
		| RET
		{
			$$ = new BlockNode();
			$$->statements.push_front($<st>1);
		}
		| /*empty*/
		{
			$$ = new BlockNode();
		}
		;

RET		: RETKW INSTR
		{
			$$ = new ReturnNode(*$2);
		}

EXPR	        : INSTR
		{
			$$ = new ExpressionStatementNode(*$1);
		}
		;

VAR		:  VARKW IDENT
		{
			$$ = new VariableDeclarationNode(*$2, 0);
		}
		| VARKW IDENT '=' I
		{
			$$ = new VariableDeclarationNode(*$2, $4);
		}
		| VARKW IDENT '=' INSTR
		{
			$$ = new VariableDeclarationNode(*$2,(ExpressionNode *)new ExpressionStatementNode(*$4));
		}
		;

FUNC	: FUNKW IDENT ARGS '{' ST '}'
		{
			$$ = new FunctionDeclarationNode(*$2, $3, *$5);
		}
		;
		
		
ARRAY		: '['V_SET']' 
			{
				$$ = new ArrayCreationNode(*$2);
			}
			| '[' ']'
			{
				ExpressionList * el = new ExpressionList();
				$$ = new ArrayCreationNode(*el);
			};		
			
V_SET		: VALUE ',' V_SET  
			{
				$$ = $3;
				$$->push_front($1);
			}
			| VALUE 
			{
				$$ = new ExpressionList();
				$$->push_front($1);
			}
			| INSTR
			{
				$$ = new ExpressionList();
				$$->push_front((ExpressionNode *)new ExpressionStatementNode(*$1));
			}
			| INSTR ',' V_SET
			{
				$$ = $3;
                                $$->push_front((ExpressionNode *)new ExpressionStatementNode(*$1));
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

PASSEDARGS	:       INSTR ',' PASSEDARGS
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

ASSIGN		: IDENT '=' INSTR
			{
				$$ = new AssignmentNode(*$1, *$3);
			};


I		:  I '+' I
		{
			$$ = new BinaryOperationNode(*$1, *$3,"sum");
		}
		| '(' INSTR ')'
		{
			$$ =(ExpressionNode*)  new ExpressionStatementNode(*$2);
		}
		| CALL
		{
			$$ =(ExpressionNode*) new ExpressionStatementNode(*$1);
		}
		| I '-' I
		{
	            $$ = new BinaryOperationNode(*$1, *$3,"subtract");
                }
		| I '*' I
		{
                    $$ = new BinaryOperationNode(*$1, *$3,"multiply");
                }
		| I '/' I
		{
                    $$ = new BinaryOperationNode(*$1, *$3,"divide");
                }
		| I '<' I
		{
                    $$ = LOWER(*$1,*$3);
                }
		| I '>' I
		{
                    $$ = GREATER(*$1,*$3);
                }
		| I LE I
		{
                    $$ = NOT(*(GREATER(*$1,*$3)));
                }
		| I GE I
		{
                    $$ = NOT(*(LOWER(*$1,*$3)));
                }
		| I EQ I
		{
                    $$ = EQUAL(*$1,*$3);
                }
		| I NE I
		{
                    $$ = NOT(*(EQUAL(*$1,*$3)));
                }
		| I OR I
		{
                    $$ = new BinaryOperationNode(*$1, *$3,"or");
                }
		| I AND I
		{
                    $$ = new BinaryOperationNode(*$1, *$3,"and");
                }
		| IDENT
		| INT
		| STR
		| ARRAY;

OBJECT		: '{' KV_SET '}' ;

KV_SET		: KV KV_SET | KV | /*empty*/ ;

KV		: IDENT ':' VALUE ';' ;

VALUE		: INT | STR | ARRAY | OBJECT;

%%
void line(char * s){
	printf("%s\n",s);
}
int yywrap() {
	return 1;
}
