#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <lib.h>

double Log2(unsigned int n)
{
    return log(n) / log(2);
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

int readInput(char* path, cacheDescription descriptor) {
	int i = 0;

	struct input input;

	printf("Abrindo arquivo de input '%s':\n", path);
	FILE * pFile = fopen(path, "r");
	if (pFile != NULL) {
		while(!feof(pFile)) {
			input = readLine(pFile, i, descriptor);
      //doStuff(input);
			i++;

			if (feof(pFile)) {
				printf("%d linhas lidas.\n", i + 1);
				printf("End of File.\n");
			}
		}
	} else {
		printf("Nao foi possivel abrir o arquivo '%s'.", path);
	}

	printf("Arquivo de input finalizado.\n");
	fclose(pFile);

	return 0;
}

struct cacheDescription readCacheDesc(char* path) {
	cacheDescription cacheDescription;

	printf("Abrindo arquivo cache description '%s':\n", path);
	FILE * pFile = fopen(path, "r");
	if (pFile != NULL) {
		while(!feof(pFile)) {
			fscanf(pFile, "line size = %d\nnumber of lines = %d\nassociativity = %d\nreplacement policy = %s\n",
			&cacheDescription.lineSize, &cacheDescription.numberOfLines, &cacheDescription.associativity, cacheDescription.replacementPolicy);
			/*printf("\tline size = %d\n", cacheDescription.lineSize);
			printf("\tnumber of lines = %d\n", cacheDescription.numberOfLines);
			printf("\tassociativity = %d\n", cacheDescription.associativity);
			printf("\treplacement policy = %s\n", cacheDescription.replacementPolicy);*/
		}
	} else {
		printf("Nao foi possivel abrir o arquivo '%s'.", path);
	}

	printf("Descricao de cache lida.\n");
	fclose(pFile);

	return cacheDescription;
}
