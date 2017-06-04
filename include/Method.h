#IFNDEF METHOD_H
#DEFINE METHOD_H

#include "Object.h"


typedef Object * (*function)(void**,int);

/*
 * A type that only responds to
 * a method 'execute'
 */
typedef struct Method {
	function f;
	const char * name;
} Method;

Method * newMethod(function f);

#ENDIF
