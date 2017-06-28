#ifndef KV_OBJECT_H
#define KV_OBJECT_H

#include <Class.h>

typedef struct Pair{
	Object * key; /*String*/
	Object * value; /*any*/
} Pair;

typedef struct KVObject {
	Pair ** pairs;
	int n;
} KVObject;

Class * KVObjectClass();

KVObject * newKVObject(void ** keys, void ** vals, int n);

#endif
