#include "include/String.h"
#include "include/Class.h"
#include "include/Method.h"
#include "include/Object.h"
#include <string.h>
#include <stdlib.h>


static Class * sClass = NULL;

/*
 * METHODS FOR String
 */
const static int NMETHODS = 5;

//[this]
Object * toStringString(void * obj, void ** args, int nArgs){
	return (Object *)obj;
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

	return sClass;
}

String * newString(const char * s){
	String * ans = (String *)malloc(sizeof(String));
	ans->s=s;
	return ans;
}
