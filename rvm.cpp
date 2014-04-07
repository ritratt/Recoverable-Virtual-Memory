#include "rvm.h"
#include <stdio.h>
#include <stdlib.h>

void* rvm_map(rvm_t rvm, const char *segname, int size_to_create) {

	if (sizeof(rvm.segment) == 0) {
		printf("Segment not created. Creating from scratch.\n");
		void* new_segment = malloc(size_to_create);
		rvm.segment = (void *) new_segment;
	}

	if (sizeof(rvm.segment) < size_to_create) {
		//Extend
		rvm.segment = realloc(&rvm.segment, (size_t) size_to_create);
	}
}
	
int main() {
	
	rvm_init(//dir);
	return 0;	
}
	
