#include <stdlib.h>
#include <stdio.h>

typedef struct Class Class;

typedef struct Object {
	void * instance;
	Class * class;
} Object;

typedef struct String {
	const char * s;
} String;

extern void * newIntegerObj(int i);

extern void * _funcexec(void * o, const char * name, void ** args, int nArgs);

int main(){
	printf("%s\n",(char *)(((String *)((Object *)_funcexec(newIntegerObj(5),"toString",NULL,0))->instance)->s));
}
