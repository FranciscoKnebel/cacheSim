#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <types.h>
#include <preprocess.h>
/*
	512 blocks / 8 associativity -> 64 sets
*/

void clearCache(cacheDescription descriptor, set sets[descriptor.setNumber]) {
	for (size_t i = 0; i < descriptor.setNumber; i++) {
		for (size_t j = 0; j < descriptor.associativity; j++) {
			(sets[i] + j)->valid = 0;
			(sets[i] + j)->tag = 0;
			(sets[i] + j)->lastAccess = 0;
			(sets[i] + j)->orderInsert = 0;
		}
	}

	return;
};

int main(int argc, char *argv[]) {
	if(argc < 4) {
		printf("\n\nFaltam argumentos para executar o programa!\n");
		printf("Exemplo: .\\program cachedescription.dat input.dat output.dat.\n\n\n");

		return 1;
	}

	char * cacheDescriptionPath = argv[1];
	char * inputFilePath = argv[2];
	char * outputFilePath = argv[3];
	struct output output;

	printf("\n----------------------------------------------------------\n");
	printf("\t\tFrancisco Paiva Knebel\t\t\t\n");
	printf("\t\t       00243688       \t\t\t\n");
	printf("\t                      \t\t\t\t\n");
	printf("\tArquivo de descricao: \t%s\t\t\n", cacheDescriptionPath);
	printf("\tArquivo de input: \t%s\t\t\n", inputFilePath);
	printf("\tArquivo de output: \t%s\t\t\n", outputFilePath);
	printf("----------------------------------------------------------\n");

	cacheDescription descriptor = readCacheDescription(cacheDescriptionPath);
	printf("\n");
	if(strcmp(descriptor.replacementPolicy, "LRU") != 0 && strcmp(descriptor.replacementPolicy, "FIFO") != 0) {
		printf("Replacement Policy '%s', lida em '%s', invalida.\n", descriptor.replacementPolicy, cacheDescriptionPath);
		printf("Valores possiveis: \n");
		printf("\t%s\n", "LRU");
		printf("\t%s\n", "FIFO");

		return 2;
	}

	block blocks[descriptor.setNumber][descriptor.associativity];
	set 	sets[descriptor.setNumber];
	cache cache;

	for (size_t i = 0; i < descriptor.setNumber; i++) {
		sets[i] = blocks[i];
	}
	cache = sets;

	clearCache(descriptor, sets);

	/* Test blocks with dereferencing sets */
	/*
		blocks[0][0].valid = true;
		blocks[0][0].tag = 123;
		blocks[0][0].lastAccess = 123;
		blocks[0][0].orderInsert = 123;

		blocks[0][1].tag = 456;
		blocks[0][1].lastAccess = 456;
		blocks[0][1].orderInsert = 456;

		blocks[0][descriptor.associativity - 1].tag = 789;
		blocks[0][descriptor.associativity - 1].lastAccess = 789;
		blocks[0][descriptor.associativity - 1].orderInsert = 789;

		blocks[1][0].tag = 900;
		blocks[1][0].lastAccess = 900;
		blocks[1][0].orderInsert = 900;

		blocks[(descriptor.setNumber) - 1][descriptor.associativity - 1].tag = 1024;
		blocks[(descriptor.setNumber) - 1][descriptor.associativity - 1].lastAccess = 1024;
		blocks[(descriptor.setNumber) - 1][descriptor.associativity - 1].orderInsert = 1024;

		printf("\n%d - %ld %d %d\n", sets[0]->valid, sets[0]->tag, sets[0]->lastAccess, sets[0]->orderInsert); // Assert 123 123 123
		printf("%d - %ld %d %d\n", (sets[0] + 1 )->valid, (sets[0] + 1 )->tag, (sets[0] + 1 )->lastAccess, (sets[0] + 1 )->orderInsert); // Assert 456 456 456
		printf("%d - %ld %d %d\n",
			(sets[0] + descriptor.associativity - 1 )->valid,
			(sets[0] + descriptor.associativity - 1 )->tag,
			(sets[0] + descriptor.associativity - 1 )->lastAccess,
			(sets[0] + descriptor.associativity - 1 )->orderInsert
		); // Assert 789 789 789
		printf("%d - %ld %d %d\n",
			(sets[1])->valid,
			(sets[1])->tag,
			(sets[1])->lastAccess,
			(sets[1])->orderInsert
		); // Assert 900 900 900
		printf("%d - %ld %d %d\n",
			(sets[(descriptor.setNumber) - 1] + descriptor.associativity - 1)->valid,
			(sets[(descriptor.setNumber) - 1] + descriptor.associativity - 1)->tag,
			(sets[(descriptor.setNumber) - 1] + descriptor.associativity - 1)->lastAccess,
			(sets[(descriptor.setNumber) - 1] + descriptor.associativity - 1)->orderInsert
		); // Assert 1024 1024 1024
	*/

	output = readInputFile(inputFilePath, descriptor, sets);
	printf("\n");

	saveOutputFile(outputFilePath, output);


	return 0;
}
