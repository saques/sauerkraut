#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/Object.h"
#include "include/Integer.h"
#include "include/String.h"
#include "include/Method.h"
#include "include/Array.h"
#include "include/KVObject.h"
#define INT0 (void *)newObject(newInteger(0),integerClass())
#define BUFSIZE 32

static int timeSet = 0;

void * newIntegerObj(int64_t i){
	return (void *)newObject(newInteger(i),integerClass());
}

void * newStringObj(const char * s){
	return (void *)newObject(newString(s),stringClass());
}

void * newKVObjectObj(void ** keys, void ** vals, int n){
	return (void *)newObject(newKVObject(keys,vals,n),KVObjectClass());
}

void * newArrayObj(void ** vals, int n){
	return (void *)newObject(newArray(vals,n),arrayClass());
}

void *  printi(void * obj) {
	Object * o_int = (Object *)_funcexec((Object *)obj,"toInt",NULL,0);
	Integer * i = (Integer *)o_int->instance;
	int64_t a = i->i;
	printf("%ld\n", a);
	return INT0;
}

void * readi(){
	int64_t i = 0;
	scanf("%ld",&i);
	return newIntegerObj(i);
}

void * read(){
	char * rec = malloc(BUFSIZE);
	scanf("%s",rec);
	return (void *)newObject(newString(rec),stringClass());
}

int eval(void * obj)
{
	Object * o = (Object *)obj;
	Object * o_int = (Object *)_funcexec(o,"toInt",NULL,0);
	Integer * i = (Integer *)o_int->instance;
	return i->i == 0 ? 0 : 1;
}

void * funcexec(void * o, char * name, void ** args, int nArgs){
	return _funcexec(o, name, args, nArgs);
}
void * print(void * v){
	Object * o = ((Object *)_funcexec((Object *)v,"toString",NULL,0));
	String * s = (String *)o->instance;
	printf("%s\n",s->s);
	return INT0;
}

void * randn(){
	if(!timeSet){
		srand(time(NULL));
		timeSet = 1;
	}
	return newObject(newInteger((int64_t)rand()),integerClass());
}
