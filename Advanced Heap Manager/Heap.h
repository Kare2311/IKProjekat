#pragma once
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <mutex>
#include "MemoryBlock.h"

class Heap {
private:
    std::vector<MemoryBlock> blocks; // Svi memorijski blokovi u ovom heap-u
    size_t totalSize;                // Ukupna velicina heap-a
    size_t usedSize;                 // Trenutno zauzeta velicina
    std::mutex lock;                 // Mutex za sinhronizaciju

public:
    Heap(size_t size);               // Konstruktor za inicijalizaciju heap-a
    void* allocate(size_t size, int userID); // Funkcija za alokaciju memorije
    bool free(void* address);         // Funkcija za dealokaciju memorije
    size_t getUsedSize() const;             // Vraca trenutno zauzetu velicinu
    size_t getFreeSize() const;             // Vraca slobodnu velicinu
};

#endif // HEAP_H