#include <stdlib.h>
#include <Class.h>

Class * newClass(const char * name, Method ** methods, int nMethods){
	Class * ans = (Class *)malloc(sizeof(Class));
	ans->name = name;
	ans->methods = methods;
	ans->nMethods = nMethods;
	return ans;
}
