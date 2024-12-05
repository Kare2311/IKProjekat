<<<<<<< Updated upstream
=======
﻿#pragma once
>>>>>>> Stashed changes
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
    AdvancedHeapManager();  // Konstruktor sa podrazumevanim vrednostima
    void* allocate(size_t size, int userID); // Round-robin alokacija memorije
<<<<<<< Updated upstream
    bool deallocate(void* address);         // Dealokacija memorije u pravilan heap
=======
    bool free(void* address);         // Dealokacija memorije u pravilan heap

    std::string getMemoryState() const;

    
>>>>>>> Stashed changes
};

#endif // ADVANCED_HEAP_MANAGER_H
