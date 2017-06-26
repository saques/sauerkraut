#IFNDEF STRING_H
#DEFINE STRING_H

#include "Class.h"

typedef struct String {
	const char * s;
} String;

Class * stringClass();

String * newString(const char * s);

#ENDIF
