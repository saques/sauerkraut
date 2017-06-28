#ifndef KV_OBJECT_H
#define KV_OBJECT_H

#include <Class.h>

typedef struct KVObject {
	struct lh_table * table;
} KVObject;

Class * KVObjectClass();

KVObject * newKVObject(void ** keys, void ** vals, int n);

#endif
