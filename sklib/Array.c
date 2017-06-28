#include <Array.h>
#include <Method.h>
#include <Object.h>
#include <Integer.h>
#include <String.h>
#include <ErrorPrint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 32

static Class * arrClass = NULL;


/*
 * METHODS FOR Array
 */
 
 const static int NMETHODS = 6;
 
 Object * sizeArr(void * this, void ** args, int nArgs){
	 if(nArgs!=0){
		 errorout("Array::size expects 0 arguments");
		 exit(1);
	 }
	 Array * a = (Array *)((Object *)this)->instance;
	 return newObject(newInteger(a->n),integerClass());
 }
 
 Object * getArr(void * this, void ** args, int nArgs){
	 if(nArgs!=1){
		 errorout("Array::get expects 1 argument");
		 exit(1);
	 }
	 Array * a = (Array *)((Object *)this)->instance;
	 Integer * index = 
	 (Integer*)_funcexec((Object *)args[0],"toInt",NULL,0)->instance;
	 if(index->i<0 || index->i>=a->n){
		 errorout("Array::get : index out of bounds");
		 exit(1);
	 }
	 return (Object *)(a->objs[index->i]);
 }
 
 Object * setArr(void * this, void ** args, int nArgs){
	 if(nArgs!=2){
		 errorout("Array::set expects 2 arguments");
		 exit(1);
	 }
	 Array * a = (Array *)((Object *)this)->instance;
	 Integer * index = 
	 (Integer*)_funcexec((Object *)args[0],"toInt",NULL,0)->instance;
	 Object * val = (Object *)args[1];
	 if(index->i<0 || index->i>=a->n){
		 errorout("Array::get : index out of bounds");
		 exit(1);
	 }
	 a->objs[index->i]=val;
	 return (Object *)this;
 }
 
 Object * toIntArr(void * this, void ** args, int nArgs){
	 if(nArgs != 0){
		 errorout("Array::toInt expects 0 arguments");
		 exit(1);
	 }
	 return sizeArr(this,args,nArgs);
 }
 
 Object * toStringArr(void * this, void ** args, int nArgs){
	 if(nArgs != 0){
		 errorout("Array::toString expects 0 arguments");
	 }
	 Array * a = (Array *)((Object *)this)->instance;
	 
	 const char ** strs = malloc(a->n);
	 
	 int len = 0;
	 for(int i=0; i<a->n; i++){
		Object * cache= (Object *)(a->objs[i]);
		cache = ((Object *)_funcexec(cache,"toString",NULL,0));
		strs[i] = ((String *)cache->instance)->s;
		len += strlen(strs[i]);
	 }  
	 
	 int commas = a->n == 0  || a->n == 1 ? 0 : a->n - 1 ; 
	 
	 char * ans = malloc(len+2+1+commas);
	 strcpy(ans,"[");
	 int i = 0 ;
	 for(;i<a->n-1;i++){
		strcat(ans,strs[i]);
		strcat(ans,",");
	 }
	 if(a->n>0){
		 strcat(ans,strs[a->n-1]);
	 }
	 strcat(ans,"]");
	 return newObject(newString(ans),stringClass());
 }
 
 Object * sumArr(void * this, void ** args, int nArgs){
	 if(nArgs != 1){
		 errorout("Array::sum expects 1 argument");
	 }
	 Array * a = (Array *)((Object *)this)->instance;
	 Object * val = (Object *)args[0];
	 
	 /*
	  * Efficiency FTW
	  */
	 a->objs = (Object **)realloc(a->objs,(a->n+1)*sizeof(Object*));
	 a->objs[a->n] = val;
	 a->n++;
	 return (Object *)this;
 }
 
 
 /*
  * END METHODS FOR Array
  */

Class * arrayClass(){
	if(arrClass!=NULL){
		return arrClass;
	}
	arrClass = newClass(ARRAY,NMETHODS);

	arrClass->methods[0] = newObject(newMethod((function)sizeArr,"size"),methodClass());
	arrClass->methods[1] = newObject(newMethod((function)getArr,"get"),methodClass());
	arrClass->methods[2] = newObject(newMethod((function)setArr,"set"),methodClass());
	arrClass->methods[3] = newObject(newMethod((function)toIntArr,"toInt"),methodClass());
	arrClass->methods[4] = newObject(newMethod((function)toStringArr,"toString"),methodClass());
	arrClass->methods[5] = newObject(newMethod((function)sumArr,"sum"),methodClass());

	return arrClass;
}

Array * newArray(void ** objs, int n){
	Array * ans = (Array *)malloc(sizeof(Array));
	ans->objs = (Object **)malloc(n*sizeof(Object *));
	for(int i=0; i<n; i++){
		ans->objs[i] = (Object *)objs[i];
	}
	ans->n = n;
	return ans;
}



