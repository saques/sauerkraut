#ifndef CLASS_H
#define CLASS_H

#include <Object.h>
typedef struct Object Object;

extern const char INTEGER[];
extern const char STRING[];
extern const char ARRAY[];
extern const char OBJECT[];
extern const char METHOD[];



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
