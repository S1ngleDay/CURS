typedef struct HashEntryNames {
    char* key;
    int address;
    int sigh;
    int size_of;
    struct HashEntry* next;
} HashEntryNames;

typedef struct HashEntryMnemo {
    char* key;
    int address;
    int sigh;
    int size_of;
    int KOP;
    struct HashEntry* next;
} HashEntryMnemo;

typedef struct {
    HashEntryMnemo ** entries;
    int size;
} HashTableMnemo;

typedef struct {
    HashEntryNames** entries;
    int size;
} HashTableNames;

int hashFunction(const char* key, int size);

HashEntryMnemo* createHashEntryMnemo(const char* key, int address, int sigh, int size_of, int KOP);
HashTableMnemo* createHashTableMnemo(int size);
void insertMnemo(HashTableMnemo * table, const char* key, int address, int sigh, int size_of, int KOP);
HashEntryMnemo* searchMnemo(HashTableMnemo* table, const char* key);
void freeHashTableMnemo(HashTableMnemo* table);
int replaceMnemo(HashTableMnemo* table, const char* key, int address, int sigh, int size_of, int KOP);
int compareEntriesMnemo(const void* a, const void* b);
HashEntryMnemo** getAllEntriesMnemo(HashTableNames* table, int* count);
void printSortedHashTableMnemo(FILE* file, HashTableNames* table);

HashEntryNames* createHashEntryNames(const char* key, int address, int sigh, int size_of);
HashTableNames* createHashTableNames(int size);
void insertNames(HashTableNames* table, const char* key, int address, int sigh, int size_of);
HashEntryNames* searchNames(HashTableNames* table, const char* key);
void freeHashTableNames(HashTableNames* table);
int replaceNames(HashTableNames* table, const char* key, int address, int sigh, int size_of);
int compareEntriesNames(const void* a, const void* b);
HashEntryNames** getAllEntriesNames(HashTableNames* table, int* count);
void printSortedHashTableNames(FILE* file, HashTableNames* table);




