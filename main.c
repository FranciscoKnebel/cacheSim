#include <stdio.h>
#include <lib.h>
#include <assert.h>

/*
	512 blocks / 8 associativity -> 64 sets
*/

int isStringNull(char * string) {
	if ((string != NULL) && (string[0] == '\0')) {
		printf("Null");
	 return 1;
	}

	printf("%s\n", string);
	return 0;
}

int main(int argc, char *argv[]) {
	if(argc < 4) {
		printf("\n\nFaltam argumentos para executar o programa!\n");
		printf("Exemplo: .\\program cachedescription.dat input.dat output.dat.\n\n\n");

		return 1;
	}

	char * cacheDescriptionPath = argv[1];
	char * inputFilePath = argv[2];
	char * outputFilePath = argv[3];

	printf("\n-----Francisco Paiva Knebel-----\n");
	printf("-----       00243688       -----\n");
	printf("\tArquivo de descricao: %s\n", cacheDescriptionPath);
	printf("\tArquivo de input: %s\n", inputFilePath);
	printf("\tArquivo de output: %s\n", outputFilePath);
	printf("-----                      -----\n");

	cacheDescription descriptor = readCacheDesc(cacheDescriptionPath);
	printf("\n");

	block blocks[descriptor.numberOfLines][descriptor.lineSize];
	set 	sets[descriptor.numberOfLines / descriptor.associativity];
	cache cache;

	for (size_t i = 0, j = 0; i < descriptor.numberOfLines / descriptor.associativity; i++,	j = j + descriptor.associativity) {
		sets[i] = blocks[j];
	}
	cache = sets;

	/* Test blocks with dereferencing sets */
	/*
		blocks[0][0].tag = 123;
		blocks[0][0].lastAccess = 123;
		blocks[0][0].bringAddress = 123;

		blocks[0][1].tag = 456;
		blocks[0][1].lastAccess = 456;
		blocks[0][1].bringAddress = 456;

		blocks[0][descriptor.lineSize - 1].tag = 789;
		blocks[0][descriptor.lineSize - 1].lastAccess = 789;
		blocks[0][descriptor.lineSize - 1].bringAddress = 789;

		blocks[descriptor.associativity][0].tag = 11;
		blocks[descriptor.associativity][0].lastAccess = 100;
		blocks[descriptor.associativity][0].bringAddress = 100;

		printf("\n%ld %d %d\n", sets[0]->tag, sets[0]->lastAccess, sets[0]->bringAddress); // Assert 123 123 123
		printf("%ld %d %d\n", (sets[0] + 1 )->tag, (sets[0] + 1 )->lastAccess, (sets[0] + 1 )->bringAddress); // Assert 456 456 456
		printf("%ld %d %d\n",
			(sets[0] + descriptor.lineSize - 1 )->tag,
			(sets[0] + descriptor.lineSize - 1 )->lastAccess,
			(sets[0] + descriptor.lineSize - 1 )->bringAddress
		); // Assert 789 789 789
		printf("%ld %d %d\n", sets[1]->tag, sets[1]->lastAccess, sets[1]->bringAddress); // Assert 11 100 100
	*/


	readInput(inputFilePath, descriptor);
	printf("\n");

	return 0;
}
