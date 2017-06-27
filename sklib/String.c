#include "include/String.h"
#include "include/Class.h"
#include "include/Method.h"
#include "include/Object.h"
#include "include/Integer.h"
#include "include/ErrorPrint.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>


static Class * sClass = NULL;

/*
 * METHODS FOR String
 */
const static int NMETHODS = 2;

Object * toStringString(void * obj, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("String::toString expects 0 arguments");
		exit(1);
	}
	return (Object *)obj;
}

Object * toIntString(void * obj, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("String::toInt expects 0 arguments");
		exit(1);
	}
	String * this = (String *)((Object *)obj)->instance;
	int i = strlen(this->s);
	return newObject(newInteger(i),integerClass());
}

/*
 * END METHODS FOR String
 */


Class * stringClass(){
	if(sClass!=NULL){
		return sClass;
	}
	sClass = newClass(STRING,NMETHODS);

	sClass->methods[0] = newObject(newMethod((function)toStringString,"toString"),methodClass());
	sClass->methods[0] = newObject(newMethod((function)toIntString,"toInt"),methodClass());

	return sClass;
}

String * newString(const char * s){
	String * ans = (String *)malloc(sizeof(String));
	ans->s=s;
	return ans;
}
