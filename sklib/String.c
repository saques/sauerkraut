#include <String.h>
#include <Class.h>
#include <Method.h>
#include <string.h>
#define NULL 0


const static Class * sClass = NULL;

/*
 * METHODS FOR String
 */
const static int NMETHODS = 5;

//[this]
Object * toString(void ** args, int nArgs){
	return args[nArgs];
}

/*
 * END METHODS FOR String
 */


Class * stringClass(){
	if(sClass!=NULL){
		return sClass;
	}
	sClass = newClass(STRING,NMETHODS);
	
	sClass->methods[0] = newObject(newMethod((function)toString,"toString"),methodClass());
	
	return sClass;
}

String * newString(const char * s){
	String * ans = (String *)malloc(sizeof(String));
	ans->s=s;
	return ans;
}
