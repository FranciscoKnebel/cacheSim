int searchInSet(struct input input, cacheDescription descriptor, set set);

void updatePolicyAll(cacheDescription descriptor, set set);
void updatePolicy(int indexOfHit, int lastAccessOfHit, cacheDescription descriptor, set set);

int readFromSet(struct input input, cacheDescription descriptor, set set, int indexOfHit);
int writeToSet(struct input input, cacheDescription descriptor, set set, int wasHit);

double Log2(unsigned int n);
struct input readLine(FILE* pFile, int i, cacheDescription descriptor);
int processLine(struct input input, cacheDescription descriptor, set sets[descriptor.numberOfLines / descriptor.associativity], struct output* output);
