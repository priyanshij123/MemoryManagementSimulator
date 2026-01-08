#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <vector>
using namespace std;

struct MemoryBlock {
    int start;
    int size;
    bool free;
    int id;
};

enum Allocator { FIRST_FIT, BEST_FIT, WORST_FIT };

extern Allocator allocatorType;

void initMemory(int size);
void allocateMemory(int size);
void freeMemory(int id);
void dumpMemory();
void showStats();

#endif
