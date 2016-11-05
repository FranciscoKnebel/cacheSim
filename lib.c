#include <stdio.h>
#include <stdlib.h>

int readLine(FILE* pFile, int i) {
	long long int number;
	char character;

	fscanf(pFile, " %lld %c\n", &number, &character);
	if(i < 64) {
		printf("\t%lld %c\n", number, character);
	} else if (i == 64) {
		printf("\t(...)\n");
	}

	if (feof(pFile)) {
		printf("\tEnd of File.\n");
	}

	return 0;
}

int readInput(char* path) {
	int i = 0;
	printf("Abrindo arquivo de input '%s':\n", path);
	FILE * pFile = fopen(path, "r");
	if (pFile != NULL) {
		while(!feof(pFile)) {
			readLine(pFile, i);
			i++;
		}
	} else {
		printf("Could not open file.");
	}

	printf("Fechando o arquivo.\n");
	fclose(pFile);

	return 0;
}

int readCacheDesc(char* path) {
	char str[80];
	int i, j, k;

	printf("Abrindo arquivo cache description '%s':\n", path);
	FILE * pFile = fopen(path, "r");
	if (pFile != NULL) {
		while(!feof(pFile)) {
			i++;
			fscanf(pFile, "line size = %d\nnumber of lines = %d\nassociativity = %d\nreplacement policy = %s", &i, &j, &k, str);
			printf("\tline size = %d\n", i);
			printf("\tnumber of lines = %d\n", j);
			printf("\tassociativity = %d\n", k);
			printf("\treplacement policy = %s\n", str);
		}
	} else {
		printf("Could not open file.");
	}

	printf("Fechando o arquivo.\n");
	fclose(pFile);

	return 0;
}
