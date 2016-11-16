#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <lib.h>
#include <stdbool.h>
#include <string.h>

double Log2(unsigned int n)
{
  return log(n) / log(2);
}

int searchInSet(struct input input, cacheDescription descriptor, set set) {
	bool isHit;
	size_t hitIndex;

	for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity && isHit != true; i++) {
		//printf("%d: Valid: %d\tTag: %lld\tLA: %d\tBA: %d\n", i, (set + i)->valid, (set + i)->tag, (set + i)->lastAccess, (set + i)->orderInsert);
		if((set + i)->tag == input.tag) { // HIT
			isHit = true;
			hitIndex = i;
		}
	}

	if(isHit) { // HIT
		return (int) hitIndex;
	} else { 		// MISS
		return -1;
	}
}

void updatePolicyAll(cacheDescription descriptor, set set) {
	for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity; i++) { // Updated policy counters
		if((set + i)->valid) {
			(set + i)->lastAccess = (set + i)->lastAccess + 1; // LRU
		}
	}
}

void updatePolicy(int indexOfHit, int lastAccessOfHit, cacheDescription descriptor, set set) {
	for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity; i++) { // Updated policy counters
		if((set + i)->valid && i != indexOfHit) {
			if((set + i)->lastAccess < lastAccessOfHit) { // LRU
				(set + i)->lastAccess = (set + i)->lastAccess + 1;
			}
		}
	}
}

int readFromSet(struct input input, cacheDescription descriptor, set set, int indexOfHit) {
	int lastAccessOfHit = (set + indexOfHit)->lastAccess;

	(set + indexOfHit)->lastAccess = 1;
	//return data from block

	updatePolicy(indexOfHit, lastAccessOfHit, descriptor, set);
}

int writeToSet(struct input input, cacheDescription descriptor, set set, int wasHit) {
	bool isSetNotFull = false;

	if(wasHit) { // elemento já existe no set.
		int indexOfHit = wasHit, lastAccessOfHit = (set + indexOfHit)->lastAccess;

		(set + indexOfHit)->valid = true;		 //
		(set + indexOfHit)->tag = input.tag; //
		(set + indexOfHit)->lastAccess = 1;

		updatePolicy(indexOfHit, lastAccessOfHit, descriptor, set);
	} else { // was Miss, logo elemento não existe no set.
		for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity && isSetNotFull == false; i++) {
			if(!(set + i)->valid) { // set não está cheio.
				updatePolicyAll(descriptor, set); //atualiza todos os elementos, pois é uma inserção

				(set + i)->valid = true;
				(set + i)->tag = input.tag;
				(set + i)->lastAccess = 1;

				isSetNotFull = true;
				break;
			}
		}

		if(!isSetNotFull) {
			/*
				Inserção de elemento, mas o set está cheio.
				Agora a inserção vai depender de qual política foi escolhida.
			*/

			//check which replacementPolicy and act
			if(strcmp(descriptor.replacementPolicy, "LRU") == 0) {
				//
			} else if (strcmp(descriptor.replacementPolicy, "FIFO") == 0) {
				//
			} else {
				return 1;
			}
		}
	}

	return 0;
}

int processLine(struct input input, cacheDescription descriptor, set sets[descriptor.numberOfLines / descriptor.associativity], struct output* output) {
	int index = searchInSet(input, descriptor, sets[input.setIndex]);
	char op;

	if(index < 0) {	// MISS
		if (input.operation == 'R') {
			output->readMisses++; // Tag not on cache. Get from lower level memory.
		} else if (input.operation == 'W') {
			writeToSet(input, descriptor, sets[input.setIndex], false);
			output->writeMisses++; // Tag not on cache.
		}
	} else {			// HIT
		if (input.operation == 'R') {
			readFromSet(input, descriptor, sets[input.setIndex], index);
			output->readHits++;
		} else if (input.operation == 'W') {
			writeToSet(input, descriptor, sets[input.setIndex], index);
			output->writeHits++;
		}
	}
}

struct input readLine(FILE* pFile, int i, cacheDescription descriptor) {
	struct input input;

	long long int address;
	char op;

	fscanf(pFile, " %lld %c\n", &address, &op);
	input.tag = (long int) (address >> (long long int) Log2(descriptor.lineSize));
	input.setIndex = address % (long long int) (descriptor.numberOfLines / descriptor.associativity );
	input.operation = op;

	return input;
}

struct output readInputFile(char* path, cacheDescription descriptor, set sets[descriptor.numberOfLines / descriptor.associativity]) {
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
