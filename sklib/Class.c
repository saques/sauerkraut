#include <stdlib.h>
#include "include/Class.h"

const char INTEGER[] = "INTEGER";
const char STRING[] = "STRING";
const char ARRAY[] = "ARRAY";
const char OBJECT[] = "OBJECT";
const char METHOD[] = "METHOD";

Class * newClass(const char * name, int nMethods){
	Class * ans = (Class *)malloc(sizeof(Class));
	ans->name = name;
	ans->methods = (Object **)malloc(sizeof(Object *)*nMethods);
	ans->nMethods = nMethods;
	return ans;
}
