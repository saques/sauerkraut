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
#include "include/linear_hash.h"
#define BUFSIZE 32
typedef struct Pair{
	Object * key; /*String*/
	Object * value; /*any*/
} Pair;

static Class * kvClass = NULL;

/*
 * METHODS FOR KVObject
 */

const static int NMETHODS = 6;

Object * contains(void * this, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Object::contains expects 1 argument");
		exit(1);
	}
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	
	Object * o = _funcexec((Object *)args[0],"toString",NULL,0);
	String * other = (String *)(o->instance);
	
	int ret = lh_exists(kv->table, (const char *)other->s);
	
	return newObject(newInteger(ret),integerClass());
}

Object * putObj(void * this, void ** args, int nArgs){
	if(nArgs!=2){
		errorout("Object::put expects 2 arguments");
		exit(1);
	}
	
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	
	Object * o_key = _funcexec((Object *)args[0],"toString",NULL,0);
	String * key = (String *)(o_key->instance);
	Object * o_val = (Object *)args[1];
	
	Pair * p = (Pair*)lh_get(kv->table, (const char *)key->s);
	if(p==0) {
		Pair * p = malloc(sizeof(Pair));
		p->key=o_key;
		p->value=o_val;
		lh_insert(kv->table, (const char*)key->s, (void*)p);
	} else {
		p->value=o_val;
	}
	return (Object *)this;
}

Object * getObj(void * this, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Object::get expects 1 argument");
		exit(1);
	}
	
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	
	Object * o_key = _funcexec((Object *)args[0],"toString",NULL,0);
	String * key = (String *)(o_key->instance);
	
	Pair * p = (Pair*)lh_get(kv->table, (const char *)key->s);
	
	if(p==0) {
		return newObject(newInteger(0),integerClass());
	}
	return p->value;
}

Object * removeObj(void * this, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Object::remove expects 1 argument");
		exit(1);
	}
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	
	Object * o_key = _funcexec((Object *)args[0],"toString",NULL,0);
	String * key = (String *)(o_key->instance);
	
	Pair * p = (Pair*)lh_delete(kv->table, (const char *)key->s);
	if(p==0){
		return newObject(newInteger(0),integerClass());
	}
	return p->value;
}

Object * sizeObj(void * this, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("Object::size expects 0 arguments");
		exit(1);
	}
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	return newObject(newInteger(lh_nelems(kv->table)),integerClass());
}

Object * toStringObj(void * this, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("Object::toString expects 0 arguments");
		exit(1);
	}
	KVObject * kv = (KVObject *)((Object *)this)->instance;
	int size = lh_nelems(kv->table);
	char * buf = malloc(BUFSIZE);
	sprintf(buf,"Object::size=%d",size);
	return newObject(newString(buf),stringClass());
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
	kvClass->methods[1] = newObject(newMethod((function)putObj,"put"),methodClass());
	kvClass->methods[2] = newObject(newMethod((function)getObj,"get"),methodClass());
	kvClass->methods[3] = newObject(newMethod((function)removeObj,"remove"),methodClass());
	kvClass->methods[4] = newObject(newMethod((function)sizeObj,"size"),methodClass());
	kvClass->methods[5] = newObject(newMethod((function)toStringObj,"toString"),methodClass());

	return kvClass;
}

KVObject * newKVObject(void ** keys, void ** vals, int n){
	KVObject * ans = (KVObject *)malloc(sizeof(KVObject));
	
	ans->table = lh_new();
	
	for(int i=0; i<n; i++){
		Pair * p = (Pair *)malloc(sizeof(Pair));
		p->key=(Object *)keys[i];
		p->value=(Object *)vals[i];
		String * key = (String *)(p->key->instance);
		if(!lh_exists(ans->table,(const char*)key->s)) {
			lh_insert(ans->table, (const char*)key->s, (void*)p);
		} else {
			lh_update(ans->table, (const char*)key->s, (void*)p);
		}
	}
	return ans;
}
