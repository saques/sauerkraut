#include <Method.h>
#include <Object.h>
#include <Integer.h>
#include <String.h>

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
	//TODO
	return 0;
}

/* debug only -> remember to do proper print */
void *  printi(void * i) {
	int a = ((Integer *)((Object *) i)->instance)->i;
	printf("%d\n", a);
	return 0;
}

void * funcexec(void * o, char * name, void ** args, int nArgs)
{
	return _funcexec(o, name, args, nArgs);
}
