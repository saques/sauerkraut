#include "include/Object.h"
#include <stdlib.h>

Object * newObject(void * instance, Class * class){
	Object * ans = (Object *)malloc(sizeof(Object));
	ans->instance = instance;
	ans->class = class;
	return ans;
}
