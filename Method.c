#include <Method.h>
#include <Class.h>

/*
 * METHODS FOR Method
 */
void * execute(Method * f, void** args, int nArgs){
	return f->f(args,nArgs);
}
/*
 * END METHODS FOR Method
 */


/*
 * This function should be executed only
 * once. Only one reference to the Class
 * should be maintained.
 */
Class * methodClass(){
	Method ** m = (Method **)malloc(sizeof(Method *)*1);
	m[0] = (function)execute;
	return newClass(METHOD,m,1);
}

Method * newMethod(function f){
	Method * ans = (Method *)malloc(sizeof(Method));
	ans->f=f;
	return ans;
}
