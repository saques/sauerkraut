#include <Integer.h>
#include <Class.h>
#include <Method.h>
#include <string.h>
#include <String.h>
#include <Object.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFSIZE 16


static Class * iClass = NULL;

/*
 * METHODS FOR Integer
 */
const static int NMETHODS = 6;

//[this]
Object * toStringInteger(void * obj, void ** args, int nArgs){
	Integer * this = (Integer *)(((Object *)args[nArgs])->instance);
	char * s = malloc(BUFSIZE);
	snprintf(s,BUFSIZE,"%d",this->i);
	return newObject(newString(s),stringClass());
}

//[other,this]
Object * sum(void * obj,void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i+other->i),integerClass());
}

//[other,this]
Object * subtract(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i-other->i),integerClass());
}

//[other,this]
Object * multiply(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i*other->i),integerClass());
}

//[other,this]
Object * divide(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i/other->i),integerClass());
}

//[other,this]
Object * modulo(void * obj,void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i%other->i),integerClass());
}
/*
 * END METHODS FOR Integer
 */

Class * integerClass(){
	if(iClass!=NULL){
		return iClass;
	}
	iClass = newClass(INTEGER,NMETHODS);

	iClass->methods[0] = newObject(newMethod((function)sum,"sum"),methodClass());
	iClass->methods[1] = newObject(newMethod((function)subtract,"subtract"),methodClass());
	iClass->methods[2] = newObject(newMethod((function)multiply,"multiply"),methodClass());
	iClass->methods[3] = newObject(newMethod((function)divide,"divide"),methodClass());
	iClass->methods[4] = newObject(newMethod((function)modulo,"modulo"),methodClass());
	iClass->methods[5] = newObject(newMethod((function)toStringInteger,"toString"),methodClass());

	return iClass;
}

Integer * newInteger(int i){
	Integer * ans = (Integer *)malloc(sizeof(Integer));
	ans->i=i;
	return ans;
}
