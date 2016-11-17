struct input readLine(FILE* pFile, int i, cacheDescription descriptor);
struct output readInputFile(char* path, cacheDescription descriptor, set sets[descriptor.setNumber]);
cacheDescription readCacheDescription(char* path);

void saveOutputFile(char* path, struct output output);
