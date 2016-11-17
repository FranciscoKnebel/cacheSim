#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <types.h>
#include <process.h>

struct input readLine(FILE* pFile, int i, cacheDescription descriptor) {
	struct input input;

	long long int address;
	char op;

	fscanf(pFile, " %lld %c\n", &address, &op);

	input.tag = address >> (long long int) log2(descriptor.lineSize);
	input.setIndex = input.tag % (descriptor.setNumber);
	input.operation = op;

	return input;
}

struct output readInputFile(char* path, cacheDescription descriptor, set sets[descriptor.setNumber]) {
	int accessCount = 0;

	struct input input;
	struct output output;

	printf("Abrindo arquivo de input '%s':\n", path);
	FILE * pFile = fopen(path, "r");
	if (pFile != NULL) {
		while(!feof(pFile)) {
			input = readLine(pFile, accessCount, descriptor);
      processLine(input, descriptor, sets, &output);
			accessCount++;

			if (feof(pFile)) {
				output.accessCount = accessCount;
			}
		}
	} else {
		printf("Nao foi possivel abrir o arquivo '%s'.", path);
	}

	printf("Arquivo de input finalizado.\n");
	fclose(pFile);

	return output;
}

struct cacheDescription readCacheDescription(char* path) {
	cacheDescription cacheDescription;

	printf("Abrindo arquivo cache description '%s':\n", path);
	FILE * pFile = fopen(path, "r");
	if (pFile != NULL) {
		while(!feof(pFile)) {
			fscanf(pFile, "line size = %d\nnumber of lines = %d\nassociativity = %d\nreplacement policy = %s\n",
			&cacheDescription.lineSize, &cacheDescription.numberOfLines, &cacheDescription.associativity, cacheDescription.replacementPolicy);
			/*
      printf("\tline size = %d\n", cacheDescription.lineSize);
			printf("\tnumber of lines = %d\n", cacheDescription.numberOfLines);
			printf("\tassociativity = %d\n", cacheDescription.associativity);
			printf("\treplacement policy = %s\n", cacheDescription.replacementPolicy);
      */
		}
		cacheDescription.setNumber = cacheDescription.numberOfLines / cacheDescription.associativity;
	} else {
		printf("Nao foi possivel abrir o arquivo '%s'.", path);
	}

	printf("Descricao de cache lida.\n");
	fclose(pFile);

	return cacheDescription;
}

void saveOutputFile(char* path, struct output output) {
	/*
		Gerar um arquivo de saída, onde em cada linha deve constar:
			o Número de acessos. (Access count:VALOR)
			o Número de write hits. (Read hits:VALOR)
			o Número de write misses. (Read misses:VALOR)
			o Número de read hits. (Write hits:VALOR)
			o Número de read misses. (Write misses:VALOR)
	*/

	FILE * pFile = fopen(path, "w");
	if (pFile != NULL) {
		fprintf(pFile, "Access count:%lld\n", output.accessCount);
		fprintf(pFile, "Read hits:%lld\n", output.readHits);
		fprintf(pFile, "Read misses:%lld\n", output.readMisses);
		fprintf(pFile, "Write hits:%lld\n", output.writeHits);
		fprintf(pFile, "Write misses:%lld\n", output.writeMisses);
	} else {
		printf("Nao foi possivel abrir o arquivo '%s'.", path);
	}

	fclose(pFile);

	return;
}
