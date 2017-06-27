#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/Integer.h"
#include "include/Class.h"
#include "include/Object.h"
#include "include/String.h"
#include "include/Method.h"
#include "include/ErrorPrint.h"

#define BUFSIZE 16


static Class * iClass = NULL;

/*
 * METHODS FOR Integer
 */
const static int NMETHODS = 13;

Object * toStringInteger(void * obj, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("Integer::toString expexts 0 arguments");
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);
	char * s = malloc(BUFSIZE);
	snprintf(s,BUFSIZE,"%d",this->i);
	return newObject(newString(s),stringClass());
}

Object * toIntInteger(void * obj, void ** args, int nArgs){
	if(nArgs!=0){
		errorout("Integer::toInt expexts 0 arguments");
		exit(1);
	}
	return (Object *)obj;
}

Object * sum(void * obj,void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Integer::sum expects 1 argument");
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);

	Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
	Integer * other = (Integer *)(o->instance);

	return newObject(newInteger(this->i+other->i),integerClass());
}

Object * subtract(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Integer::subtract expects 1 argument");
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);

	Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
	Integer * other = (Integer *)(o->instance);

	return newObject(newInteger(this->i-other->i),integerClass());
}

Object * multiply(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Integer::multiply expects 1 argument");
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);

	Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
	Integer * other = (Integer *)(o->instance);

	return newObject(newInteger(this->i*other->i),integerClass());
}

Object * divide(void * obj, void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Integer::divide expects 1 argument");
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);

	Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
	Integer * other = (Integer *)(o->instance);

	return newObject(newInteger(this->i/other->i),integerClass());
}

Object * modulo(void * obj,void ** args, int nArgs){
	if(nArgs!=1){
		errorout("Integer::modulo expects 1 argument");
		exit(1);
	}
	Integer * this = (Integer *)(((Object *)obj)->instance);

	Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
	Integer * other = (Integer *)(o->instance);

	return newObject(newInteger(this->i%other->i),integerClass());
}

//[other,this]
Object * equal(void * obj,void ** args, int nArgs){
    if(nArgs!=1){
        errorout("Integer::equal expects 1 argument");
        exit(1);
    }
    Integer * this = (Integer *)(((Object *)obj)->instance);

    Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
    Integer * other = (Integer *)(o->instance);

    return newObject(newInteger(this->i==other->i),integerClass());
}
//[other,this]
Object * lower(void * obj,void ** args, int nArgs){
    if(nArgs!=1){
        errorout("Integer::lower expects 1 argument");
        exit(1);
    }
    Integer * this = (Integer *)(((Object *)obj)->instance);

    Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
    Integer * other = (Integer *)(o->instance);

    return newObject(newInteger(this->i<other->i),integerClass());
}
//[other,this]
Object * greater(void * obj,void ** args, int nArgs){
    if(nArgs!=1){
        errorout("Integer::greater expects 1 argument");
        exit(1);
    }
    Integer * this = (Integer *)(((Object *)obj)->instance);

    Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
    Integer * other = (Integer *)(o->instance);

    return newObject(newInteger(this->i>other->i),integerClass());
}

//[other,this]
Object * or(void * obj,void ** args, int nArgs){
    if(nArgs!=1){
        errorout("Integer::or expects 1 argument");
        exit(1);
    }
    Integer * this = (Integer *)(((Object *)obj)->instance);

    Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
    Integer * other = (Integer *)(o->instance);

    return newObject(newInteger(this->i||other->i),integerClass());
}

//[other,this]
Object * and(void * obj,void ** args, int nArgs){
    if(nArgs!=1){
        errorout("Integer::and expects 1 argument");
        exit(1);
    }
    Integer * this = (Integer *)(((Object *)obj)->instance);

    Object * o = _funcexec((Object *)args[0],"toInt",NULL,0);
    Integer * other = (Integer *)(o->instance);

    return newObject(newInteger(this->i&&other->i),integerClass());
}

//[other,this]
Object * not(void * obj,void ** args, int nArgs){
    if(nArgs!=0){
        errorout("Integer::not expects 0 argument");
        exit(1);
    }
    Integer * this = (Integer *)(((Object *)obj)->instance);
    return newObject(newInteger(!this->i),integerClass());
}
/*
 * END METHODS FOR Integer
 */

Class * integerClass(){
	if(iClass!=NULL){
		return iClass;
	}
	iClass = newClass(INTEGER,NMETHODS);

	iClass->methods[0] = newObject(newMethod((function)sum,"sum"),methodClass());
	iClass->methods[1] = newObject(newMethod((function)subtract,"subtract"),methodClass());
	iClass->methods[2] = newObject(newMethod((function)multiply,"multiply"),methodClass());
	iClass->methods[3] = newObject(newMethod((function)divide,"divide"),methodClass());
	iClass->methods[4] = newObject(newMethod((function)modulo,"modulo"),methodClass());
	iClass->methods[5] = newObject(newMethod((function)equal,"equal"),methodClass());
	iClass->methods[6] = newObject(newMethod((function)not,"not"),methodClass());
	iClass->methods[7] = newObject(newMethod((function)lower,"lower"),methodClass());
	iClass->methods[8] = newObject(newMethod((function)greater,"greater"),methodClass());
	iClass->methods[9] = newObject(newMethod((function)or,"or"),methodClass());
	iClass->methods[10] = newObject(newMethod((function)and,"and"),methodClass());
	iClass->methods[11] = newObject(newMethod((function)toStringInteger,"toString"),methodClass());
    iClass->methods[12] = newObject(newMethod((function)toIntInteger,"toInt"),methodClass());

	return iClass;
}

Integer * newInteger(int i){
	Integer * ans = (Integer *)malloc(sizeof(Integer));
	ans->i=i;
	return ans;
}
