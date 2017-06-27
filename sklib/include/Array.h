#ifndef ARRAY_H
#define ARRAY_H

#include <Object.h>

typedef struct Array {
	Object ** objs ;
	int n;
} Array ;

Class * arrayClass();

Array * newArray(void ** objs, int n);

#endif
