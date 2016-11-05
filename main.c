#include <stdio.h>
#include <lib.h>

int main() {
	char* path = "input.dat";
	char* desc = "cachedesc.dat";

	readInput(path);
	printf("\n");
	readCacheDesc(desc);
	printf("\n");

	return(0);
}
