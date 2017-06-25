#include "Integer.h"
#include "Class.h"
#include "Method.h"
#include <string.h>


/*
 * METHODS FOR Integer
 */
const static int NMETHODS = 5;
//[other,this]
Integer * sum(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = args[1];
	Integer * other = args[0];
	return newInteger(this->i+other->i);
}

//[other,this]
Integer * subtract(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = args[1];
	Integer * other = args[0];
	return newInteger(this->i-other->i);
}

//[other,this]
Integer * multiply(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = args[1];
	Integer * other = args[0];
	return newInteger(this->i*other->i);
}

//[other,this]
Integer * divide(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = args[1];
	Integer * other = args[0];
	return newInteger(this->i/other->i);
}

//[other,this]
Integer * modulo(void ** args, int nArgs){
	if(nArgs!=1){
		exit(1);
	}
	Integer * this = args[1];
	Integer * other = args[0];
	return newInteger(this->i%other->i);
}

/*
 * END METHODS FOR Integer
 */


/*
 * This function should be executed only
 * once. Only one reference to the Class
 * should be maintained.
 */
Class * integerClass(){
	Method ** m = (Method **)malloc(sizeof(Method *)*NMETHODS);
	m[0] = newMethod((function)add,"add");
	m[1] = newMethod((function)subtract,"subtract");
	m[2] = newMethod((function)multiply,"multiply");
	m[3] = newMethod((function)divide,"divide");
	m[4] = newMethod((function)modulo,"modulo");
	return newClass(METHOD,m,NMETHODS);
}

Method * newInteger(int i){
	Integer * ans = (Integer *)malloc(sizeof(Integer));
	ans->i=i;
	return ans;
}
