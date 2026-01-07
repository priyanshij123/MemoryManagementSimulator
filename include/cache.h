#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <queue>

using namespace std;

struct CacheLine {
    int tag;
    bool valid;
};

struct CacheSet {
    vector<CacheLine> lines;
    queue<int> fifo;
};

struct Cache {
    int size;
    int blockSize;
    int associativity;
    int numSets;
    vector<CacheSet> sets;
    int hits;
    int misses;
};

extern Cache L1;
extern Cache L2;

void initCache(Cache &c, int size, int blockSize, int assoc);
void accessMemory(int address);

#endif
