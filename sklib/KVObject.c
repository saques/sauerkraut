#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/Integer.h"
#include "include/Class.h"
#include "include/Object.h"
#include "include/String.h"
#include "include/Method.h"
#include "include/ErrorPrint.h"
#include "include/KVObject.h"

static Class * kvClass = NULL;

/*
 * METHODS FOR KVObject
 */

const static int NMETHODS = 1;

Object * contains(void * this, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Object::contains expects 1 argument");
		exit(1);
	}
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	
	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);
	
	for(int i=0; i<kv->n; i++){
		Pair * p = kv->pairs[i];
		String * key = (String*)_funcexec(p->key,"toString",NULL,0)->instance;
		if(strcmp((const char*)key->s,(const char*)other->s)==0){
			return newObject(newInteger(1),integerClass());
		}
	}
	return newObject(newInteger(0),integerClass());
}


/*
 * END METHODS FOR KVObject
 */
 
 
Class * KVObjectClass(){
	if(kvClass!=NULL){
		return kvClass;
	}
	
	kvClass = newClass(OBJECT,NMETHODS);

	kvClass->methods[0] = newObject(newMethod((function)contains,"contains"),methodClass());

	return kvClass;
}

KVObject * newKVObject(void ** keys, void ** vals, int n){
	KVObject * ans = (KVObject *)malloc(sizeof(KVObject));
	
	ans->pairs = malloc(sizeof(Pair*)*n);
	
	for(int i=0; i<n; i++){
		Pair * p = (Pair *)malloc(sizeof(Pair));
		p->key=(Object *)keys[i];
		p->value=(Object *)vals[i];
		ans->pairs[i]=p;
	}
	ans->n = n;
	return ans;
}
