#ifndef OBJECT_H
#define OBJECT_H

#include "Class.h"

/*
 * Variables must be assigned
 * an instance of this struct.
 * 
 * The idea is to have a sort
 * of Map<Identifier,Object>
 * that can be dynamically 
 * modified. Assignments should
 * be represented in said map.
 * 
 * 'instance' points to one
 * of the types specified by
 * Sauerkraut:
 *  + Integer
 *  + String
 *  + Array
 *  + KVObject(As in javascript)
 *  + Method
 */ 
typedef struct Object {
	void * instance;
	Class * class;
} Object;

/*
 * Usage:
 * newObject(newInteger(5),integerClass);
 */
Object * newObject(void * instance, Class * class);
#endif
