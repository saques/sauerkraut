#ifndef INTEGER_H
#define INTEGER_H

#include <Class.h>
#include <stdint.h>

typedef struct Integer {
	int64_t i;
} Integer;

Class * integerClass();

Integer * newInteger(int64_t i);

#endif
