#ifndef CLASS_H
#define CLASS_H

#include <Object.h>
typedef struct Object Object;

const char INTEGER[] = "INTEGER";
const char STRING[] = "STRING";
const char ARRAY[] = "ARRAY";
const char OBJECT[] = "OBJECT";
const char METHOD[] = "METHOD";



typedef struct Class {
	const char * name;
	Object ** methods;
	int nMethods;
} Class;

/*
 * Methods must be filled one by one in each
 * new{Integer|String|KVObject|Array} function
 */
Class * newClass(const char * name, int nMethods);

#endif
