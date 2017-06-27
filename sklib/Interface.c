#include <stdio.h>
#include "include/Object.h"
#include "include/Integer.h"
#include "include/String.h"
#include "include/Method.h"
#include "include/Array.h"

void * newIntegerObj(int i){
	return (void *)newObject(newInteger(i),integerClass());
}

void * newStringObj(const char * s){
	return (void *)newObject(newString(s),stringClass());
}

void * newKVObjectObj(char ** keys, void ** vals, int n){
	//TODO
	return 0;
}

void * newArrayObj(void ** vals, int n){
	return (void *)newObject(newArray(vals,n),arrayClass());
}

/* debug only -> remember to do proper print */
void *  printi(void * i) {
	int a = ((Integer *)((Object *) i)->instance)->i;
	printf("%d\n", a);
	return 0;
}

void * funcexec(void * o, char * name, void ** args, int nArgs){
	return _funcexec(o, name, args, nArgs);
}
void * print(void * v){
	Object * o = ((Object *)_funcexec((Object *)v,"toString",NULL,0));
	String * s = (String *)o->instance;
	printf("%s\n",s->s);
	return 0;
}
