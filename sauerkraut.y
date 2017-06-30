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
	KVObjectCreationNode * obj_cr;
	std::deque<VariableDeclarationNode*> *arg_list;
	std::deque<ExpressionNode*> *expr_list;
	std::deque<ExpressionNode*> *arr_list;
	int i;
	char * s;
}
%token ID INTEGER STRING VARKW FUNKW END LE GE EQ NE OR AND WHILEKW EXTERNKW RETKW IFKW ELSEKW

%type <st>	    FUNC VAR EXPR EXTERN_FUNC RET IF WHILE
%type <ident> 	IDENT
%type <block> 	ST
%type <arg_list> ARGS ARGSET
%type <expr>	INSTR VALUE INT CALL I STR ASSIGN ARRAY OBJECT
%type <obj_cr>  KV_SET
%type <expr_list> PASSEDARGS PASSEDARGSPREV
%type <i> INTEGER EXTERN_FUNC_ARGS
%type <s> ID STRING

%type <arr_list> V_SET


%right "="

%left OR
%left AND
%left '>' '<' LE GE EQ NE
%left '+' '-'
%left '*' '/' '%'
%right '!'
%left '.'
%left '['
%%

S		: ST
		{
			programStart = $1;
		}
		;

ST		: EXPR DELIM ST
		{
			$$ = $3;
			$3->statements.push_front($<st>1);
		}
		| EXTERN_FUNC DELIM ST
		{
			$$ = $3;
			$3->statements.push_front($<st>1);
		}
		| FUNC DELIM ST
		{
			$$ = $3;
			$3->statements.push_front($<st>1);
		}
		| VAR DELIM ST
		{
			$$ = $3;
			$3->statements.push_front($<st>1);
		}
		| IF DELIM ST
		{

			$$ = $3;
			$3->statements.push_front($<st>1);
		}
		|	WHILE DELIM ST
		{
			$$ = $3;
			$3->statements.push_front($<st>1);
		}
		| RET DELIM
		{
			$$ = new BlockNode();
			$$->statements.push_front($<st>1);
		}
		| DELIM ST
		{
			$$ = $2;
		}
		| /*empty*/
		{
			$$ = new BlockNode();
		}
		;

DELIM	:	';'
		| '\n'
		;

RET		: RETKW I
		{
			$$ = new ReturnNode(*$2);
		}

EXPR	: INSTR
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
		;

FUNC	: FUNKW IDENT ARGS '{' ST '}'
		{
			$$ = new FunctionDeclarationNode(*$2, $3, *$5);
		}
		;


OBJECT		: '{' KV_SET '}'
			 {
				$$ = $2;
			 }
			 ;

KV_SET		:  STR ':' I DELIM KV_SET
			 {
				$$ = $5;
				$$->keys.push_front($1);
				$$->values.push_front($3);
			 }
			 | DELIM KV_SET
			 {
				 $$ = $2;
			 }
			 | /* End of declaration block */
			 {
				ExpressionList * keys = new ExpressionList();
 				ExpressionList * values = new ExpressionList();
				$$ = new KVObjectCreationNode(*keys,*values);
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

V_SET		: I ',' V_SET
			{
				$$ = $3;
				$$->push_front($1);
			}
			| I
			{
				$$ = new ExpressionList();
				$$->push_front($1);
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
			;

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

INSTR		: I | ASSIGN ;


CALL		:  IDENT '(' PASSEDARGSPREV ')'
			{
				$$ = new FunctionCallNode(*$1, *$3);
			}
			| I'.'IDENT'('PASSEDARGSPREV')'
			{
				$$ = new MethodInvocationNode(*$1,*$3,*$5);
			}
			;

PASSEDARGSPREV : PASSEDARGS
			   |
			   {
				   $$ = new ExpressionList();
			   }
			   ;

PASSEDARGS	:   I ',' PASSEDARGS
		        {
		                $$ = $3;
			        $$->push_front($1);
		        }
		        | I
		        {
				$$ = new ExpressionList();
				$$->push_front($1);
				}
			;


WHILE		: WHILEKW '(' INSTR ')' '{' ST '}'
			{
				$$ = new WhileNode(*$3, *$6);
			}
			;

IF	:	IFKW	'(' INSTR ')' '{' ST '}' ELSEKW '{' ST '}'
 	{
		$$ = new IfNode(*$3, *$6, *$10);
	}
	| IFKW '(' INSTR ')' '{' ST '}'
	{
		BlockNode * elseBlock = new BlockNode();
		$$ = new IfNode(*$3, *$6, *elseBlock);
	}
	;

ASSIGN		: IDENT '=' I
			{
				$$ = new AssignmentNode(*$1, *$3);
			};


I		: I '+' I
		{
			$$ = new BinaryOperationNode(*$1, *$3,"sum");
		}
		| '(' I ')'
		{
			$$ = $2;
		}
		| CALL
		{
			$$ = $1;
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
        |
          I '%' I
        {
			$$ = new BinaryOperationNode(*$1, *$3,"modulo");
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
		| I AND I
		{
			$$ = new BinaryOperationNode(*$1, *$3,"and");
		}
		| I OR I
		{
			$$ = new BinaryOperationNode(*$1, *$3,"or");
		}
		|'!' I	%prec '*'
  		{
			$$ = NOT(*$2);
		}
		| '-' I %prec '*'
		{
			$$ =  new BinaryOperationNode((ExpressionNode&)*(new IntegerNode(0)), *$2,"subtract");
		}
		| I '[' I ']'
		{
			$$ = new BinaryOperationNode(*$1, *$3, "get");
		}
		| IDENT
		{
			$$ = (ExpressionNode *) $1;
		}
		| VALUE;

VALUE		: INT | STR | ARRAY | OBJECT
            ;

%%
void line(char * s){
	printf("%s\n",s);
}
int yywrap() {
	return 1;
}
