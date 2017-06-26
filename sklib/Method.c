#include <Method.h>
#include <Class.h>
#include <Object.h>
#include <string.h>
#include <stdio.h>
#define NULL 0

const static Class * mClass = NULL;

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
Object * execute(void ** args, int nArgs){
	Method * f = (Method *)(((Object *)args[nArgs])->instance);
	return f->f(args,nArgs);
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
			void ** cArgs = realloc(args, sizeof(void *)(nArgs + 1));
			cArgs[nArgs] = (void *)o;
			return m->f(cArgs,nArgs);
		}
	}
	/*
	 * No method has matched
	 */
	fprintf(stderr,"No method with name %s has been found\n",name);
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
	mClass = newClass(METHOD,NMETHODS);
	
	mClass->methods[0] = newObject(newMethod((function)execute,"execute"),mClass);
	
	return mClass;
}

Method * newMethod(function f, const char * name){
	Method * ans = (Method *)malloc(sizeof(Method));
	ans->f=f;
	ans->name=name;
	return ans;
}
