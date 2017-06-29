#include "include/Method.h"
#include "include/Class.h"
#include "include/Object.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static Class * mClass = NULL;

/*
 * METHODS FOR Method
 * Last argument (args[nArgs])
 * is the Method instance.
 *
 * Dimension of 'args' is assured
 * to be nArgs+1 by _funcexec function below
 */

const static int NMETHODS = 1;

//[args(0)...args(nArgs-1),this]
Object * execute(void *arg, void ** args, int nArgs){
	Method * f = (Method *)(((Object *)arg)->instance);
	return f->f(arg, args,nArgs);
}

/*
 * END METHODS FOR Method
 */


/*
 * This is what is called when a function token
 * has been detected.
 *
 * Example:
 *
 * <CODE IN SAUERKRAUT>
 *
 * var f = @(a,b)
 *  ...
 * end
 *
 * f(5,"five")
 *
 * <>
 *
 * When this is detected, it should be translated to C
 * as (pseudocode):
 *
 * _funcexec(f,"execute",{a,b},2);
 *
 * Then, a method with name "execute" will be
 * searched in the list of methods that the class
 * Method accepts.
 *
 * f(5,"five") and f.execute(5,"five") are synonyms
 */
Object * _funcexec(Object * o, const char * name, void ** args, int nArgs){
	Class * c = o->class;
	Method * m = NULL;
	for(int i=0; i<c->nMethods; i++){
		m = (Method *)((Object *)c->methods[i])->instance;
		if(strcmp(name,m->name)==0){
			return m->f(o,args,nArgs);
		}
	}
	/*
	 * No method has matched
	 */
	fprintf(stderr,"%s : method %s not found\n",c->name,name);
	exit(1);
}

/*
 * This function should be executed only
 * once. Only one reference to the Class
 * should be maintained.
 */
Class * methodClass(){
	if(mClass!=NULL){
		return mClass;
	}
	mClass = (Class *)malloc(sizeof(Class));
	
	mClass->methods = (Object **)malloc(sizeof(Object *)*NMETHODS);
	mClass->nMethods = NMETHODS;

	mClass->methods[0] = newObject(newMethod((function)execute,"execute"),mClass);

	return mClass;
}

Method * newMethod(function f, const char * name){
	Method * ans = (Method *)malloc(sizeof(Method));
	ans->f=f;
	ans->name=name;
	return ans;
}
