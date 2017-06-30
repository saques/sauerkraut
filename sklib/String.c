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
const static int NMETHODS = 9;

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

	int len = this->len;
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

Object * stringLength(void * obj, void ** args, int nArgs) {
	if(nArgs!=0){
		errorout("String::length expects 0 arguments");
		exit(1);
	}

	String * this = (String *)((Object *)obj)->instance;
	const char * s = this->s;
	int64_t len = (int64_t) this->len;
	return newObject(newInteger(len), integerClass());
}

Object * sumString(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("String::sum expects 1 arguments");
		exit(1);
	}
	String * this = (String *)((Object *)obj)->instance;

	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);

	char * ans = malloc(this->len+other->len+1);

	strcpy(ans,this->s);
	strcat(ans,other->s);

	return newObject(newString(ans),stringClass());
}

Object * getString(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("String::get expects 1 argument");
		exit(1);
	}
	String * str = (String *)((Object *)obj)->instance;
	Integer * index = (Integer*)_funcexec((Object *)args[0],"toInt",NULL,0)->instance;

	int len = str->len;
	if(index->i<0 || index->i >= len){
		errorout("String::get : index out of bounds");
		exit(1);
	}
	char c = str->s[index->i];
	char arr [] = {c, 0};
	return newObject(newString( (char *) arr),stringClass());
}

Object * setString(void * obj, void ** args, int nArgs){
	if(nArgs!=2){
		errorout("String::set expects 2 arguments");
		exit(1);
	}
	String * str = (String *)((Object *)obj)->instance;
	Integer * index = (Integer*)_funcexec((Object *)args[0],"toInt",NULL,0)->instance;

	int len = str->len;
	if(index->i<0 || index->i >= len){
		errorout("String::get : index out of bounds");
		exit(1);
	}

	String * value = (String*)_funcexec((Object *)args[1],"toString",NULL,0)->instance;
	if (value->len != 1) {
		errorout("String::set expects a single character string");
		exit(1);
	}
	char * dup = strdup(str->s);
	if (dup == NULL) {
		errorout("Memory error expects 1 arguments");
		exit(1);
	}
	dup[index->i] = value->s[0];
	free((void*) str->s);
	str->s = dup;
	return (Object *) obj;
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
	sClass->methods[6] = newObject(newMethod((function)stringLength,"length"),methodClass());
	sClass->methods[7] = newObject(newMethod((function)getString,"get"),methodClass());
	sClass->methods[8] = newObject(newMethod((function)setString,"set"),methodClass());
	return sClass;
}

String * newString(const char * s){
	String * ans = (String *)malloc(sizeof(String));
	char * c_string = strdup(s);
	if (c_string == NULL) {
		errorout("Memory error");
		exit(1);
	}
	ans->s=c_string;
	ans->len = strlen(c_string);
	return ans;
}
