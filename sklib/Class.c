#include <stdlib.h>
#include "include/Class.h"
#include "include/ErrorPrint.h"
#include "include/Object.h"
#include "include/String.h"
#include "include/Method.h"

const char INTEGER[] = "Integer";
const char STRING[] = "String";
const char ARRAY[] = "Array";
const char OBJECT[] = "Object";
const char METHOD[] = "Method";

Object * gc = NULL;

void * getClass(void * t, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("Class::class expects 0 arguments");
		exit(1);
	}
	Object * this = (Object *)t;
	Class * c = this->class;
	return newObject(newString(c->name),stringClass());
}


Class * newClass(const char * name, int nMethods){
	if(gc == NULL){
		gc = newObject(newMethod((function)getClass,"class"),methodClass());
	}
	
	Class * ans = (Class *)malloc(sizeof(Class));
	ans->name = name;
	ans->methods = (Object **)malloc(sizeof(Object *)*nMethods+1);
	ans->methods[nMethods] = gc ;
	ans->nMethods = nMethods+1;
	
	return ans;
}
