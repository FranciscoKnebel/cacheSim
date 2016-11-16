#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#include <types.h>

double Log2(unsigned int n) {
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
			(set + i)->orderInsert = (set + i)->orderInsert + 1; // FIFO
		}
	}
}

void updatePolicyLRU(int indexOfHit, int lastAccessOfHit, cacheDescription descriptor, set set) {
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

	updatePolicyLRU(indexOfHit, lastAccessOfHit, descriptor, set);
}

int insertElementLRU(struct input input, cacheDescription descriptor, set set) {
	/*
		O set está completamente cheio, e está sendo pedido a inserção de um elemento nessa lista.
		Será necessário retirar o elemento com o maior lastAccess da lista, que deve ter valor igual à lineSize * associativity.
		Alterar os elementos desse bloco para os valores do novo elemento, e alterar o lastAccess para 1.
		Todos os outros elementos agora deverão ser incrementados em 1, pois não há valores com lastAccess maior do que o antigo.
	*/
	char op;

	updatePolicyAll(descriptor, set);

	for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity; i++) {
		printf("%lld: %lld - %lld ", (set + i)->tag, (set + i)->lastAccess, descriptor.lineSize * descriptor.associativity);

		if((set + i)->lastAccess >= (descriptor.lineSize * descriptor.associativity)) {
			printf("LRU: Trocando %lld por %lld: ", (set + i)->tag, input.tag);

			(set + i)->valid = true;
			(set + i)->tag = input.tag;
			(set + i)->lastAccess = 1;
		}
	}
	scanf("%c", &op);

	return 0;
}

int insertElementFIFO(struct input input, cacheDescription descriptor, set set) {
	/*
		O set está completamente cheio, pedindo uma inserção de elemento na lista.
		O elemento com maior orderInsert (que deve ser igual à lineSize * associativity) do set será removido da lista,
		pois ele foi o "primeiro" a ser inserido, e o "primeiro" que deve ser removido.
		Incrementar em um o valor de orderInsert de todos os elementos do set, e o maior será substituído
		pelo novo elemento, recebendo orderInsert 1.
	*/

	updatePolicyAll(descriptor, set);

	for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity; i++) {
		if((set + i)->orderInsert >= (descriptor.lineSize * descriptor.associativity)) {
			printf("FIFO: Trocando %lld por %lld: ", (set + i)->tag, input.tag);

			(set + i)->valid = true;
			(set + i)->tag = input.tag;
			(set + i)->orderInsert = 1;
		}
	}

	return 0;
}

int writeToSet(struct input input, cacheDescription descriptor, set set, int wasHit) {
	if(wasHit) { // elemento já existe no set.
		int indexOfHit = wasHit, lastAccessOfHit = (set + indexOfHit)->lastAccess;

		(set + indexOfHit)->valid = true;		 //
		(set + indexOfHit)->tag = input.tag; //
		(set + indexOfHit)->lastAccess = 1;

		updatePolicyLRU(indexOfHit, lastAccessOfHit, descriptor, set);
	} else { // was Miss, logo elemento não existe no set.
		bool isSetFull = true;
		char op;

		for (size_t i = 0; i < descriptor.lineSize * descriptor.associativity && isSetFull == true; i++) {
			//printf("%d: Valid: %d\tTag: %lld\tLA: %d\tBA: %d\n", i, (set + i)->valid, (set + i)->tag, (set + i)->lastAccess, (set + i)->orderInsert);

			if(!(set + i)->valid) { // Inserção de elemento e o set não está cheio.
				updatePolicyAll(descriptor, set); // atualiza todos os elementos, pois é uma inserção

				(set + i)->valid = true;
				(set + i)->tag = input.tag;
				(set + i)->lastAccess = 1;
				(set + i)->orderInsert = 1;

				isSetFull = false; // indica que o set não está cheio.
				break;
			}
		}

		if(isSetFull) {
			/*
				Inserção de elemento, mas o set está cheio.
				Agora a inserção vai depender de qual política foi escolhida.
			*/
			if(strcmp(descriptor.replacementPolicy, "LRU") == 0) {
				//
				insertElementLRU(input, descriptor, set);
			} else if (strcmp(descriptor.replacementPolicy, "FIFO") == 0) {
				//elemento com maior orderIsert será substituido
				insertElementFIFO(input, descriptor, set);
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
