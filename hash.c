#include <stdlib.h>
#include <string.h>
#include <printf.h>
#include "hash.h"

HashEntryMnemo* createHashEntryMnemo(const char* key, int address, int sigh, int size_of, int KOP) {
    HashEntryMnemo * entry = (HashEntryMnemo*)malloc(sizeof(HashEntryMnemo));
    entry->key = strdup(key);
    entry->address = address;
    entry->sigh = sigh;
    entry->size_of = size_of;
    entry->KOP = KOP;
    entry->next = NULL;
    return entry;
}

HashTableMnemo* createHashTableMnemo(int size) {
    HashTableMnemo* table = (HashTableMnemo*)malloc(sizeof(HashTableMnemo));
    table->size = size;
    table->entries = (HashEntryMnemo**)malloc(sizeof(HashEntryMnemo*) * size);
    for (int i = 0; i < table->size; ++i)
        table->entries[i] = NULL;
    return table;
}

int hashFunction(const char* key, int size) {
    int sum = 0;
    for (int i = 0; key[i] != '\0'; ++i) {
        sum += key[i];
    }
    return sum % size;
}

void insertMnemo(HashTableMnemo * table, const char* key, int address, int sigh, int size_of, int KOP) {
    int index = hashFunction(key, table->size);
    HashEntryMnemo* entry = createHashEntryMnemo(key, address, sigh, size_of, KOP);
    entry->next = table->entries[index];
    table->entries[index] = entry;
}

HashEntryMnemo* searchMnemo(HashTableMnemo* table, const char* key) {
    int index = hashFunction(key, table->size);
    HashEntryMnemo* current = table->entries[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int replaceMnemo(HashTableMnemo* table, const char* key, int address, int sigh, int size_of, int KOP) {
    int index = hashFunction(key, table->size);
    HashEntryMnemo* current = table->entries[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->address = address;
            current->sigh = sigh;
            current->size_of = size_of;
            current->KOP = KOP;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freeHashTableMnemo(HashTableMnemo* table) {
    for (int i = 0; i < table->size; i++) {
        HashEntryMnemo* current = table->entries[i];
        while (current != NULL) {
            HashEntryMnemo* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->entries);
    free(table);
}

HashEntryNames* createHashEntryNames(const char* key, int address, int sigh, int size_of) {
    HashEntryNames* entry = (HashEntryNames*)malloc(sizeof(HashEntryNames));
    entry->key = strdup(key);
    entry->address = address;
    entry->sigh = sigh;
    entry->size_of = size_of;
    entry->next = NULL;
    return entry;
}

HashTableNames* createHashTableNames(int size) {
    HashTableNames* table = (HashTableNames*)malloc(sizeof(HashTableNames));
    table->size = size;
    table->entries = (HashEntryNames**)malloc(sizeof(HashEntryNames*) * size);
    for (int i = 0; i < table->size; ++i)
        table->entries[i] = NULL;
    return table;
}

void insertNames(HashTableNames* table, const char* key, int address, int sigh, int size_of) {
    int index = hashFunction(key, table->size);
    HashEntryNames* entry = createHashEntryNames(key, address, sigh, size_of);
    entry->next = table->entries[index];
    table->entries[index] = entry;
}

HashEntryNames* searchNames(HashTableNames* table, const char* key) {
    int index = hashFunction(key, table->size);
    HashEntryNames* current = table->entries[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int replaceNames(HashTableNames* table, const char* key, int address, int sigh, int size_of) {
    int index = hashFunction(key, table->size);
    HashEntryNames* current = table->entries[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            current->address = address;
            current->sigh = sigh;
            current->size_of = size_of;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void freeHashTableNames(HashTableNames* table) {
    for (int i = 0; i < table->size; i++) {
        HashEntryNames* current = table->entries[i];
        while (current != NULL) {
            HashEntryNames* temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(table->entries);
    free(table);
}

int compareEntriesNames(const void* a, const void* b) {
    HashEntryNames* entryA = *(HashEntryNames**)a;
    HashEntryNames* entryB = *(HashEntryNames**)b;
    return (entryA->address - entryB->address);
}

HashEntryNames** getAllEntriesNames(HashTableNames* table, int* count) {
    int totalEntries = 0;
    for (int i = 0; i < table->size; i++) {
        HashEntryNames* entry = table->entries[i];
        while (entry != NULL) {
            totalEntries++;
            entry = entry->next;
        }
    }
    HashEntryNames** entries = malloc(totalEntries * sizeof(HashEntryNames*));
    int index = 0;
    for (int i = 0; i < table->size; i++) {
        HashEntryNames* entry = table->entries[i];
        while (entry != NULL) {
            entries[index++] = entry;
            entry = entry->next;
        }
    }
    *count = totalEntries;
    return entries;
}

void printSortedHashTableNames(FILE* file, HashTableNames* table) {
    int count;
    HashEntryNames** entries = getAllEntriesNames(table, &count);
    qsort(entries, count, sizeof(HashEntryNames*), compareEntriesNames);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%12s %12d %12x\n", entries[i]->key, entries[i]->sigh, entries[i]->address);
    }

    free(entries);
}

int compareEntriesMnemo(const void* a, const void* b) {
    HashEntryMnemo* entryA = *(HashEntryMnemo**)a;
    HashEntryMnemo* entryB = *(HashEntryMnemo**)b;
    return (entryA->KOP - entryB->KOP);
}

HashEntryMnemo** getAllEntriesMnemo(HashTableNames* table, int* count) {
    int totalEntries = 0;
    for (int i = 0; i < table->size; i++) {
        HashEntryNames* entry = table->entries[i];
        while (entry != NULL) {
            totalEntries++;
            entry = entry->next;
        }
    }
    HashEntryMnemo** entries = malloc(totalEntries * sizeof(HashEntryMnemo*));
    int index = 0;
    for (int i = 0; i < table->size; i++) {
        HashEntryNames* entry = table->entries[i];
        while (entry != NULL) {
            entries[index++] = entry;
            entry = entry->next;
        }
    }
    *count = totalEntries;
    return entries;
}

void printSortedHashTableMnemo(FILE* file, HashTableNames* table) {
    int count;
    HashEntryMnemo** entries = getAllEntriesMnemo(table, &count);
    qsort(entries, count, sizeof(HashEntryMnemo*), compareEntriesMnemo);
    for (int i = 0; i < count; i++) {
        fprintf(file, "%12s %12d %12x\n", entries[i]->key, entries[i]->sigh, entries[i]->KOP);
    }
    free(entries);
}
