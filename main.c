#include <stdio.h>
#include <lib.h>

/*
	512 blocks / 8 associativity -> 64 sets
*/

int isStringNull(char * string) {
	printf("1");
	if ((string != NULL) && (string[0] == '\0')) {
		printf("Null");
	 return 1;
	}

	printf("%s\n", string);
	return 0;
}

int main(int argc, char *argv[]) {
	char * cacheDescriptionPath = argv[1];
	char * inputFilePath = argv[2];
	char * outputFilePath = argv[3];

	printf("\n-----Francisco Paiva Knebel-----\n");
	printf("-----       00243688       -----\n");
	printf("\tArquivo de descrição: %s\n", cacheDescriptionPath);
	printf("\tArquivo de input: %s\n", inputFilePath);
	printf("\tArquivo de output: %s\n", outputFilePath);
	printf("-----                      -----\n");

	//isStringNull(cacheDescriptionPath);

	/*if(isStringNull(cacheDescriptionPath) == 0 || isStringNull(inputFilePath) == 0 || isStringNull(outputFilePath) == 0) {
		printf("Path invalida de arquivo.");
		return 1;
	};*/



	cacheDescription descriptor = readCacheDesc(cacheDescriptionPath);
	printf("\n");

	block blocks[descriptor.numberOfLines][descriptor.lineSize];
	set 	sets[descriptor.numberOfLines / descriptor.associativity];
	cache cache;

	readInput(inputFilePath, descriptor);
	printf("\n");

	return 0;
}
