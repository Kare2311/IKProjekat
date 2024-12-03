#pragma once
#ifndef ADVANCED_HEAP_MANAGER_H
#define ADVANCED_HEAP_MANAGER_H

#include <vector>
#include <mutex>
#include "Heap.h"

class AdvancedHeapManager {
private:
    std::vector<Heap> heaps;       // Niz heap-ova
    size_t heapCount;              // Ukupan broj heap-ova
    size_t currentHeapIndex;       // Indeks trenutnog heap-a za round-robin
    std::mutex lock;               // Mutex za sinhronizaciju pristupa

public:
    AdvancedHeapManager(size_t heapCount, size_t heapSize); // Konstruktor
    void* allocate(size_t size, int userID); // Round-robin alokacija memorije
    bool free(void* address);         // Dealokacija memorije u pravilan heap
};

#endif // ADVANCED_HEAP_MANAGER_H

