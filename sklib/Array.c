#include <Array.h>
#include <Method.h>
#include <Object.h>
#include <Integer.h>
#include <String.h>
#include <ErrorPrint.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFSIZE 32

static Class * arrClass = NULL;


/*
 * METHODS FOR Array
 */
 
 const static int NMETHODS = 5;
 
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
	 Integer * index = (Integer *)((Object *)args[0])->instance;
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
	 Integer * index = (Integer *)((Object *)args[0])->instance;
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
	 char * s = malloc(BUFSIZE);
	 sprintf(s,"Array: Size = %d",a->n);
	 return newObject(newString(s),stringClass());
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

	return arrClass;
}

Array * newArray(void ** objs, int n){
	Array * ans = (Array *)malloc(sizeof(Array));
	ans->objs = (Object **)objs;
	ans->n = n;
	return ans;
}



