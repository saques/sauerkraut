#include "include/String.h"
#include "include/Class.h"
#include "include/Method.h"
#include "include/Object.h"
#include "include/Integer.h"
#include "include/ErrorPrint.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


static Class * sClass = NULL;

/*
 * METHODS FOR String
 */
const static int NMETHODS = 6;

bool isnumber(const char * s) {
		while (*s != 0 && *s == ' ') s++;
		if (*s == 0 || ((*s<'0' || *s>'9') && *s!='.') ) {
			return false;
		}
		while (*s != 0 && *s >= '0' && *s<= '9') s++;
		if (*s != 0 && *s != '.') {
			return false;
		}
		*s++;
		while (*s != 0 && *s >= '0' && *s<= '9') s++;
		while (*s != 0 && *s == ' ') s++;
		if (*s == 0) {
			return true;
		}
		return false;
}

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
	const char * s = this->s;

	int len = strlen(s);
	char * tmp = (char*)malloc(len+1);
	strcpy(tmp,s);
	for(int i=0; i<len; i++){
		tmp[i]=(char)tolower((char)tmp[i]);
	}

	if(strcmp("true",tmp)==0){
		free((void*)tmp);
		return newObject(newInteger(1),integerClass());
	} else if(strcmp("false",tmp)==0){
		free((void*)tmp);
		return newObject(newInteger(0),integerClass());
	} else {
		free((void*)tmp);
		int64_t stringInt = (int64_t) atof(s);
		if (stringInt == 0 && !isnumber(s)) {
			errorout("String::toInt could not convert to Integer");
			exit(1);
		}
		return newObject(newInteger(stringInt), integerClass());
	}
}

Object * sumString(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("String::sum expects 1 arguments");
		exit(1);
	}
	String * this = (String *)((Object *)obj)->instance;

	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);

	char * ans = malloc(strlen(this->s)+strlen(other->s)+1);

	strcpy(ans,this->s);
	strcat(ans,other->s);

	return newObject(newString(ans),stringClass());
}

Object * Strequal(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("String::equal expects 1 arguments");
		exit(1);
	}
	String * this = (String *)((Object *)obj)->instance;

	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);

	return newObject(newInteger(strcmp(this->s,other->s)==0),integerClass());
}

Object * Strlower(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("String::lower expects 1 arguments");
		exit(1);
	}
	String * this = (String *)((Object *)obj)->instance;

	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);

	return newObject(newInteger(strcmp(this->s,other->s)<0),integerClass());
}

Object * Strgreater(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("String::greater expects 1 arguments");
		exit(1);
	}
	String * this = (String *)((Object *)obj)->instance;

	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);

	return newObject(newInteger(strcmp(this->s,other->s)>0),integerClass());
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
	sClass->methods[1] = newObject(newMethod((function)toIntString,"toInt"),methodClass());
	sClass->methods[2] = newObject(newMethod((function)Strequal,"equal"),methodClass());
	sClass->methods[3] = newObject(newMethod((function)Strlower,"lower"),methodClass());
	sClass->methods[4] = newObject(newMethod((function)Strgreater,"greater"),methodClass());
	sClass->methods[5] = newObject(newMethod((function)sumString,"sum"),methodClass());

	return sClass;
}

String * newString(const char * s){
	String * ans = (String *)malloc(sizeof(String));
	char * c_string = strdup(s);
	ans->s=c_string;
	return ans;
}
