#ifndef METHOD_H
#define METHOD_H

#include "Object.h"
#include "Class.h"


typedef Object * (*function)(void**,int);

/*
 * A type that only responds to
 * a method 'execute'
 */
typedef struct Method {
	function f;
	const char * name;
} Method;

Class * methodClass();

Method * newMethod(function f, const char * name);

#endif
