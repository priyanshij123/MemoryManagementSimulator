#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <climits>

#include "../include/allocator.h"
#include "../include/cache.h"

using namespace std;

vector<MemoryBlock> memoryBlocks;
int totalMemory = 0;
int nextId = 1;
int totalRequests = 0;
int successRequests = 0;
int failedRequests = 0;
int internalFragmentation = 0;

Allocator allocatorType = FIRST_FIT;

Cache L1, L2;

/* =====================================================
   MEMORY FUNCTIONS
   ===================================================== */

void initMemory(int size) {
    memoryBlocks.clear();
    memoryBlocks.push_back({0, size, true, -1});
    totalMemory = size;
    nextId = 1;

    totalRequests = successRequests = failedRequests = internalFragmentation = 0;
    cout << "Initialized memory with size " << size << " bytes\n";
}

int findFreeBlock(int size) {
    int index = -1;

    if (allocatorType == FIRST_FIT) {
        for (int i = 0; i < memoryBlocks.size(); i++)
            if (memoryBlocks[i].free && memoryBlocks[i].size >= size)
                return i;
    }

    if (allocatorType == BEST_FIT) {
        int best = INT_MAX;
        for (int i = 0; i < memoryBlocks.size(); i++)
            if (memoryBlocks[i].free && memoryBlocks[i].size >= size &&
                memoryBlocks[i].size < best) {
                best = memoryBlocks[i].size;
                index = i;
            }
    }

    if (allocatorType == WORST_FIT) {
        int worst = -1;
        for (int i = 0; i < memoryBlocks.size(); i++)
            if (memoryBlocks[i].free && memoryBlocks[i].size >= size &&
                memoryBlocks[i].size > worst) {
                worst = memoryBlocks[i].size;
                index = i;
            }
    }

    return index;
}

void allocateMemory(int size) {
    totalRequests++;
    int idx = findFreeBlock(size);

    if (idx == -1) {
        failedRequests++;
        cout << "Allocation failed\n";
        return;
    }

    MemoryBlock freeBlock = memoryBlocks[idx];
    MemoryBlock used = {freeBlock.start, size, false, nextId++};
    MemoryBlock remaining = {
        freeBlock.start + size,
        freeBlock.size - size,
        true,
        -1
    };

    internalFragmentation += freeBlock.size - size;

    memoryBlocks.erase(memoryBlocks.begin() + idx);
    memoryBlocks.insert(memoryBlocks.begin() + idx, used);
    if (remaining.size > 0)
        memoryBlocks.insert(memoryBlocks.begin() + idx + 1, remaining);

    successRequests++;
    cout << "Allocated block id=" << used.id
         << " at address=0x"
         << hex << setw(4) << setfill('0') << used.start << dec << endl;
}

void freeMemory(int id) {
    for (auto &b : memoryBlocks) {
        if (!b.free && b.id == id) {
            b.free = true;
            b.id = -1;
            break;
        }
    }

    for (int i = 0; i < memoryBlocks.size() - 1; i++) {
        if (memoryBlocks[i].free && memoryBlocks[i + 1].free) {
            memoryBlocks[i].size += memoryBlocks[i + 1].size;
            memoryBlocks.erase(memoryBlocks.begin() + i + 1);
            i--;
        }
    }

    cout << "Block " << id << " freed and merged\n";
}

void dumpMemory() {
    for (auto &b : memoryBlocks) {
        cout << "[0x" << hex << setw(4) << setfill('0') << b.start
             << " - 0x" << setw(4) << b.start + b.size - 1 << dec << "] ";
        if (b.free) cout << "FREE\n";
        else cout << "USED (id=" << b.id << ")\n";
    }
}

void showStats() {
    int used = 0, freeMem = 0, largestFree = 0;

    for (auto &b : memoryBlocks) {
        if (b.free) {
            freeMem += b.size;
            largestFree = max(largestFree, b.size);
        } else used += b.size;
    }

    cout << "Total memory: " << totalMemory << endl;
    cout << "Used memory: " << used << endl;
    cout << "Memory utilization: "
         << (double)used / totalMemory * 100 << "%\n";
    cout << "Internal fragmentation: "
         << internalFragmentation << " bytes\n";
    cout << "External fragmentation: "
         << (double)(freeMem - largestFree) / totalMemory * 100 << "%\n";
    cout << "Allocation success rate: "
         << (double)successRequests / totalRequests * 100 << "%\n";
}

/* =====================================================
   CACHE FUNCTIONS
   ===================================================== */

void initCache(Cache &c, int size, int blockSize, int assoc) {
    c.size = size;
    c.blockSize = blockSize;
    c.associativity = assoc;
    // Standard formula: NumSets = TotalSize / (BlockSize * Associativity)
    c.numSets = size / (blockSize * assoc);
    
    c.hits = 0;
    c.misses = 0;

    c.sets.assign(c.numSets, CacheSet());
    for (auto &s : c.sets) {
        s.lines.assign(assoc, {0, false});
       
        while(!s.fifo.empty()) s.fifo.pop();
    }
}

bool accessCache(Cache &c, int address) {

    int blockAddr = address / c.blockSize;  
    int setIdx = blockAddr % c.numSets;     
    int tag = blockAddr / c.numSets;        

    

    CacheSet &s = c.sets[setIdx];

    for (int i = 0; i < c.associativity; i++) {
        if (s.lines[i].valid && s.lines[i].tag == tag) {
            c.hits++;
            return true;
        }
    }

    c.misses++;
    int replaceIdx;

    if (s.fifo.size() < (size_t)c.associativity) {
       
        replaceIdx = s.fifo.size();
    } else {
      
        replaceIdx = s.fifo.front();
        s.fifo.pop();
    }

    s.lines[replaceIdx].tag = tag;
    s.lines[replaceIdx].valid = true;
    s.fifo.push(replaceIdx); 

    return false;
}

void accessMemory(int address) {
   
    if (accessCache(L1, address)) {
        cout << "L1 HIT\n";
        return;
    }
    cout << "L1 MISS\n";

    if (accessCache(L2, address)) {
        cout << "L2 HIT\n";
        return;
    }
    cout << "L2 MISS -> Main Memory\n";
}

/* =====================================================
   MAIN
   ===================================================== */

int main() {
    cout << "Memory Management + Cache Simulator\n";

int l1_size, l1_block, l1_assoc;
int l2_size, l2_block, l2_assoc;

cout << "Enter L1 cache size, block size, associativity: ";
cin >> l1_size >> l1_block >> l1_assoc;

cout << "Enter L2 cache size, block size, associativity: ";
cin >> l2_size >> l2_block >> l2_assoc;

initCache(L1, l1_size, l1_block, l1_assoc);
initCache(L2, l2_size, l2_block, l2_assoc);


    string cmd;
    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "init") {
            string tmp; int size;
            cin >> tmp >> size;
            initMemory(size);
        }
        else if (cmd == "set") {
            string tmp, type;
            cin >> tmp >> type;
            if (type == "first_fit") allocatorType = FIRST_FIT;
            else if (type == "best_fit") allocatorType = BEST_FIT;
            else allocatorType = WORST_FIT;
        }
        else if (cmd == "malloc") {
            int size; cin >> size;
            allocateMemory(size);
        }
        else if (cmd == "free") {
            int id; cin >> id;
            freeMemory(id);
        }
        else if (cmd == "dump") {
            string tmp; cin >> tmp;
            dumpMemory();
        }
        else if (cmd == "stats") {
            showStats();
        }
        else if (cmd == "access") {
            int addr; cin >> addr;
            accessMemory(addr);
        }
        else if (cmd == "exit") break;
    }
    return 0;
}
