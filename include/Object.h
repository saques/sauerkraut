#IFNDEF OBJECT_H
#DEFINE OBJECT_H

#include Class.h

/*
 * Variables must be assigned
 * an instance of this struct.
 * 
 * The idea is to have a sort
 * of Map<Identifier,Object>
 * that can be dynamically 
 * modified. Assugnments should
 * be represented in said map.
 * 
 * 'instance' points to one
 * of the types specified by
 * Sauerkraut:
 *  + Integer
 *  + String
 *  + Array
 *  + Object
 *  + Method
 */ 
typedef struct Object {
	void * instance;
	Class * class;
} Object;

Object * newObject(void * instance, Class * class);
#ENDIF
