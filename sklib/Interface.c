#include <Method.h>
#include <Object.h>
#include <Integer.h>
#include <String.h>

void * newInteger(int i){
	return (void *)newObject(newInteger(i),integerClass());
}

void * newString(const char * s){
	return (void *)newObject(newString(s),stringClass());
}

void * newKVObject(char ** keys, void ** vals, int n){
	//TODO
	return 0;
}

void * newArray(void ** vals, int n){
	//TODO
	return 0;
}
