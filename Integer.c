#include <Integer.h>
#include <Class.h>
#include <Method.h>
#include <string.h>
#include <String.h>
#include <Object.h>
#include <stdlib.h>
#define BUFSIZE 16


extern Class * integerClass;
extern Class * stringClass;

/*
 * METHODS FOR Integer
 */
const static int NMETHODS = 6;

//[this]
Object * toString(void ** args, int nArgs){
	Integer * other = (Integer *)(((Object *)args[nArgs])->instance);
	char * s = malloc(BUFSIZE);
	snprintf(s,BUFSIZE,"%d",this->i);
	return newObject(newString(s),stringClass);
}

//[other,this]
Object * sum(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i+other->i),integerClass);
}

//[other,this]
Object * subtract(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i-other->i),integerClass);
}

//[other,this]
Object * multiply(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i*other->i),integerClass);
}

//[other,this]
Object * divide(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i/other->i),integerClass);
}

//[other,this]
Object * modulo(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)args[1])->instance);
	Integer * other = (Integer *)(((Object *)args[0])->instance);
	return newObject(newInteger(this->i%other->i),integerClass);
}
/*
 * END METHODS FOR Integer
 */

/*
 * This function should be executed only
 * once. Only one reference to the Class
 * should be maintained.
 */
Class * integerClass(){
	Method ** m = (Method **)malloc(sizeof(Method *)*NMETHODS);
	m[0] = newMethod((function)add,"add");
	m[1] = newMethod((function)subtract,"subtract");
	m[2] = newMethod((function)multiply,"multiply");
	m[3] = newMethod((function)divide,"divide");
	m[4] = newMethod((function)modulo,"modulo");
	m[5] = newMethod((function)toString,"toString");
	return newClass(METHOD,m,NMETHODS);
}

Method * newInteger(int i){
	Integer * ans = (Integer *)malloc(sizeof(Integer));
	ans->i=i;
	return ans;
}
