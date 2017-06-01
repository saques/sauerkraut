#define VARIABLE 0
#define IDENTIFIER 1
#define NUMBER 2
#define STRING 3
/*
 * The idea is to signal that the
 * array has begun. The values of
 * the array should be identified
 * afterwards, until the end mark
 * has not been read. Both arrays
 * and objects (the value part)
 * can contain any types previou-
 * sly defined.
 */
#define ARRAY_BEGIN 4
/*
 * This is the endmark of the ar-
 * ray
 */
#define ARRAY_END 5
/*
 * Same concept as in arrays.
 */
#define OBJECT_BEGIN 6
#define OBJECT_END 7
/*
 * This is the mark that separat-
 * es values in an array and key-
 * value pairs in an object.
 */
#define COMMA 8
/*
 * This mark separates key from
 * value in an object.
 */
#define COLON 9
#define FUNCTION 10
#define WHILE 11
#define IF 12
/*
 * End mark for blocks of code.
 */
#define END 13
#define EQ 14
#define LT 15
#define GT 16
#define LE 17
#define GE 18
#define NOT 19
/*
 * A function that can be as-
 * signed to variables.
 */
#define ANONYMOUS_FUNCTION 20
