struct cacheDescription {
  unsigned int lineSize;
  unsigned int numberOfLines;
  unsigned int associativity;
  char replacementPolicy[10];
};
typedef struct cacheDescription cacheDescription;

struct block {
	_Bool valid;
  long int tag;
  int lastAccess;
  int bringAddress;
};
typedef struct block block;
typedef struct block * set;
typedef struct block ** cache;

struct input {
  long int tag;
  int setIndex;
  char operation;
};

struct output {
	unsigned long int accessCount;
	unsigned long int writeHits;
	unsigned long int writeMisses;
	unsigned long int readHits;
	unsigned long int readMisses;
};

struct input readLine(FILE* pFile, int i, cacheDescription descriptor);
struct output readInputFile(char* path, cacheDescription descriptor, set sets[descriptor.numberOfLines / descriptor.associativity]);
cacheDescription readCacheDescription(char* path);

void saveOutputFile(char* path, struct output output);
