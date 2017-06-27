#include <ErrorPrint.h>
#include <stdio.h>

void errorout(const char * error){
	fprintf(stderr,"%s\n",error);
}
