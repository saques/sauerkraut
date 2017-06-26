#ifndef METHOD_H
#define METHOD_H

#include <Object.h>
#include <Class.h>


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

/*
 * Function that executes methods of a given object
 * if the class of said object contains a method with
 * the given name. Otherwise, exit(1).
 * 
 * INTERFACE METHOD
 */
Object * _funcexec(Object * o, const char * name, void ** args, int nArgs);

#endif
