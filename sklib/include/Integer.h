#ifndef INTEGER_H
#define INTEGER_H

#include <Class.h>

typedef struct Integer {
	int i;
} Integer;

Class * integerClass();

Integer * newInteger(int i);

#endif
