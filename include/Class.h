#IFNDEF CLASS_H
#DEFINE CLASS_H

#include "Method.h"



const char INTEGER[] = "INTEGER";
const char STRING[] = "STRING";
const char ARRAY[] = "ARRAY";
const char OBJECT[] = "OBJECT";
const char METHOD[] = "METHOD";


typedef struct Class {
	char * name;
	/*
	 * Method instances
	 * should be singletons
	 */
	Method ** methods;
	int nMethods;
} Class;


Class * newClass(const char * name, Method ** methods, int nmethods);

#ENDIF
