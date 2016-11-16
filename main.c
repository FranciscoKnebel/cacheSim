#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <types.h>
#include <preprocess.h>
/*
	512 blocks / 8 associativity -> 64 sets
*/

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

	block blocks[descriptor.numberOfLines][descriptor.lineSize];
	set 	sets[descriptor.numberOfLines / descriptor.associativity];
	cache cache;

	for (size_t i = 0, j = 0; i < descriptor.numberOfLines / descriptor.associativity; i++,	j = j + descriptor.associativity) {
		sets[i] = blocks[j];
	}
	cache = sets;

	/* Test blocks with dereferencing sets */
	/*
		blocks[0][0].valid = true;
		blocks[0][0].tag = 123;
		blocks[0][0].lastAccess = 123;
		blocks[0][0].orderInsert = 123;

		blocks[0][1].tag = 456;
		blocks[0][1].lastAccess = 456;
		blocks[0][1].orderInsert = 456;

		blocks[0][descriptor.lineSize - 1].tag = 789;
		blocks[0][descriptor.lineSize - 1].lastAccess = 789;
		blocks[0][descriptor.lineSize - 1].orderInsert = 789;

		blocks[1][0].tag = 900;
		blocks[1][0].lastAccess = 900;
		blocks[1][0].orderInsert = 900;

		blocks[descriptor.associativity - 1][descriptor.lineSize - 1].tag = 500;
		blocks[descriptor.associativity - 1][descriptor.lineSize - 1].lastAccess = 500;
		blocks[descriptor.associativity - 1][descriptor.lineSize - 1].orderInsert = 500;

		blocks[descriptor.associativity][0].tag = 11;
		blocks[descriptor.associativity][0].lastAccess = 100;
		blocks[descriptor.associativity][0].orderInsert = 100;

		printf("\n%d - %ld %d %d\n", sets[0]->valid, sets[0]->tag, sets[0]->lastAccess, sets[0]->orderInsert); // Assert 123 123 123
		printf("%d - %ld %d %d\n", (sets[0] + 1 )->valid, (sets[0] + 1 )->tag, (sets[0] + 1 )->lastAccess, (sets[0] + 1 )->orderInsert); // Assert 456 456 456
		printf("%d - %ld %d %d\n",
			(sets[0] + descriptor.lineSize - 1 )->valid,
			(sets[0] + descriptor.lineSize - 1 )->tag,
			(sets[0] + descriptor.lineSize - 1 )->lastAccess,
			(sets[0] + descriptor.lineSize - 1 )->orderInsert
		); // Assert 789 789 789
		printf("%d - %ld %d %d\n",
			(sets[0] + descriptor.lineSize )->valid,
			(sets[0] + descriptor.lineSize )->tag,
			(sets[0] + descriptor.lineSize )->lastAccess,
			(sets[0] + descriptor.lineSize )->orderInsert
		); // Assert 900 900 900
		printf("%d - %ld %d %d\n",
			(sets[0] + descriptor.lineSize * descriptor.associativity - 1)->valid,
			(sets[0] + descriptor.lineSize * descriptor.associativity - 1)->tag,
			(sets[0] + descriptor.lineSize * descriptor.associativity - 1)->lastAccess,
			(sets[0] + descriptor.lineSize * descriptor.associativity - 1)->orderInsert
		); // Last of the set. Assert 500 500 500
		printf("%d - %ld %d %d\n", sets[1]->valid, sets[1]->tag, sets[1]->lastAccess, sets[1]->orderInsert); // First of the set. Assert 11 100 100
	*/

	if(strcmp(descriptor.replacementPolicy, "LRU") != 0 && strcmp(descriptor.replacementPolicy, "FIFO") != 0) {
		printf("Replacement Policy '%s', lida em '%s', invalida.\n", descriptor.replacementPolicy, cacheDescriptionPath);
		printf("Valores possiveis: \n");
		printf("\t%s\n", "LRU");
		printf("\t%s\n", "FIFO");

		return 2;
	}


	output = readInputFile(inputFilePath, descriptor, sets);
	printf("\n");

	saveOutputFile(outputFilePath, output);

	return 0;
}
