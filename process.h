int searchInSet(struct input input, cacheDescription descriptor, set set);

void updatePolicyAll(cacheDescription descriptor, set set);
void updatePolicyLRU(int indexOfHit, int lastAccessOfHit, cacheDescription descriptor, set set);

int readFromSet(struct input input, cacheDescription descriptor, set set, int indexOfHit);
int writeToSet(struct input input, cacheDescription descriptor, set set, int wasHit);

int processLine(struct input input, cacheDescription descriptor, set sets[descriptor.setNumber], struct output* output);
