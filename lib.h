struct cacheDescription {
  unsigned int lineSize;
  unsigned int numberOfLines;
  unsigned int associativity;
  char replacementPolicy[10];
};
typedef struct cacheDescription cacheDescription;

struct block {
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

struct input readLine(FILE* pFile, int i, cacheDescription descriptor);
int readInput(char* path, cacheDescription descriptor);
cacheDescription readCacheDesc(char* path);
