#include <iostream>
#include "memory_utils.h"

#define ARRAYS_COUNT 8
#define NUM_OF_STRIDES 4

void experimentThree() {
	using std::cout;
	using std::endl;
	FILE *file;
	char** mallocs;
	unsigned int write_size = 150;
	char filename[] = "exp.csv";
	file = fopen(filename,"w");
	if (!file) {
		printf("There was a problem");
		exit(1);
	}
	unsigned int written = write_megabyte(&mallocs, write_size, NULL);
	if (written!=write_size) {
		printf("Error!");
		exit(1);
	}	
	
	read_all_memory(&mallocs, write_size, file);
	printf("Now freeing...\n");
	for (unsigned int i = 0; i < write_size; i++) {
		free(mallocs[i]);
	}
	free(mallocs);
	fclose(file);
}


int main(int argc, char **argv, char **envp) {
	experimentThree();
	return 0;
}
