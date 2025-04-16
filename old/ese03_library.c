/*
 * ese03_library.c
 */
#include "ese03_library.h"
#include <stdio.h>

void printArguments(int size, char* arguments[]) {
	printf("\n");
	
	for(int i=0; i < size; i++) {
		
		printf(  "                                    argument[%d] : %s\n",   i  ,   arguments[i]   );
    
	}
	
	printf("\n\n");
}


