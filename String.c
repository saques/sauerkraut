#include <String.h>
#include <Class.h>
#include <Method.h>
#include <string.h>

/*
 * METHODS FOR String
 */
const static int NMETHODS = 5;
//[this]
Object * toString(void ** args, int nArgs){
	return args[nArgs];
}

/*
 * END METHODS FOR String
 */


Class * stringClass(){
	Method ** m = (Method **)malloc(sizeof(Method *)*NMETHODS);
	m[0] = newMethod((function)toString,"toString");
	return newClass(METHOD,m,NMETHODS);
}

Method * newInteger(const char * s){
	String * ans = (String *)malloc(sizeof(String));
	ans->s=s;
	return ans;
}
