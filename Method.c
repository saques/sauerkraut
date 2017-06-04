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
 * PSEUDOCODE:
 * 
 * This is what is called when a function token
 * has been detected.
 * 
 * Example:
 * 
 * <CODE IN SAUERKRAUT>
 * 
 * var f = @(a,b)
 *  ...
 * end
 * 
 * f(5,"five")
 * 
 * <>
 * 
 * When this is detected, it should be translated to C
 * as (pseudocode):
 * 
 * _funcexec(f,"execute",{a,b},2);
 * 
 * Then, a method with name "execute" will be
 * searched in the list of methods that the class
 * Method accepts.
 * 
 * f(5,"five") and f.execute(5,"five") are synonyms
 */  
void * _funcexec(Object * o, const char * name, void ** args, int nArgs){
	Class * c = o->class;
	for(Method * m in Class->Methods){
		if(m.name.equals(name)){
			return m->f(o,args,nArgs);
		}
	} 
}

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

Method * newMethod(function f, const char * name){
	Method * ans = (Method *)malloc(sizeof(Method));
	ans->f=f;
	ans->name=name;
	return ans;
}
