#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <vector>

using namespace std;

/* Memory block header */
struct MemoryBlock {
    int start;
    int size;
    bool free;
    int id;
};

/* Allocation strategies */
enum Allocator { FIRST_FIT, BEST_FIT, WORST_FIT };

/* Global allocator type */
extern Allocator allocatorType;

/* Functions */
void initMemory(int size);
void allocateMemory(int size);
void freeMemory(int id);
void dumpMemory();
void showStats();

#endif
