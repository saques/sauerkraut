#include <stdlib.h>
#include <Class.h>

Class * newClass(const char * name, int nMethods){
	Class * ans = (Class *)malloc(sizeof(Class));
	ans->name = name;
	ans->methods = (Object **)malloc(sizeof(Object *)*nMethods);
	ans->nMethods = nMethods;
	return ans;
}
