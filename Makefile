all:
	yacc -d sauerkraut.y
	lex sauerkraut.l
	gcc -o sauerkraut lex.yy.c y.tab.c -ly
