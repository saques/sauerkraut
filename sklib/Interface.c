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
